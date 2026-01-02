// Copyright Epic Games, Inc. All Rights Reserved.

#include "GenesisCharacter.h"
#include "GenesisAttributeSet.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h" // Added for Respawn
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Genesis.h"

AGenesisCharacter::AGenesisCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Create GAS Components
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UGenesisAttributeSet>(TEXT("AttributeSet"));
}

void AGenesisCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGenesisCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGenesisCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGenesisCharacter::Look);
	}
	else
	{
		UE_LOG(LogGenesis, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AGenesisCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Input Setup
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
	}

	// GAS Setup
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

// --- DEATH SYSTEM IMPLEMENTATION ---

void AGenesisCharacter::Die()
{
	if (HasAuthority())
	{
		Multicast_Death();

		// Save the controller so we don't lose it!
		DeathController = GetController();

		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AGenesisCharacter::Respawn, 5.0f, false);
	}
}

void AGenesisCharacter::Multicast_Death_Implementation()
{
	// 1. Disable Input
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	// 2. Disable Capsule Collision (So other players don't hit the invisible capsule)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 3. Call the Blueprint Event (For Ragdoll/VFX)
	BP_OnDeath();
}

void AGenesisCharacter::Respawn()
{
	if (HasAuthority())
	{
		// 1. Check if we have the controller saved
		if (DeathController)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attempting to respawn Controller: %s"), *DeathController->GetName());

			// CRITICAL FIX: Detach the brain from the dead body first!
			if (DeathController->GetPawn() == this)
			{
				DeathController->UnPossess();
			}

			// 2. Ask GameMode to spawn a new body
			if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
			{
				GM->RestartPlayer(DeathController);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Respawn Failed: DeathController is NULL!"));
		}

		// 3. Destroy the old corpse
		Destroy();
	}
}

// --- STANDARD MOVEMENT CODE ---

void AGenesisCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AGenesisCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AGenesisCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AGenesisCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AGenesisCharacter::DoJumpStart()
{
	Jump();
}

void AGenesisCharacter::DoJumpEnd()
{
	StopJumping();
}

UAbilitySystemComponent* AGenesisCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}