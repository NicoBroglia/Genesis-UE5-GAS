// Copyright Epic Games, Inc. All Rights Reserved.

#include "GenesisCharacter.h"
#include "Genesis.h"
#include "GenesisAttributeSet.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

// Define the Log Category declared in Genesis.h
DEFINE_LOG_CATEGORY(LogGenesis);

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

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UGenesisAttributeSet>(TEXT("AttributeSet"));
    
    bIsDead = false;
}

// --- REPLICATION ---

void AGenesisCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AGenesisCharacter, bIsDead);
}

// --- GAS INIT ---

void AGenesisCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

void AGenesisCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

// --- INPUT BINDING ---

void AGenesisCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGenesisCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGenesisCharacter::Look);
    }
}

void AGenesisCharacter::BeginPlay()
{
    Super::BeginPlay();

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
}

// --- DEATH SYSTEM ---

void AGenesisCharacter::Die()
{
    if (HasAuthority() && !bIsDead)
    {
        bIsDead = true;
        DeathController = GetController();
        OnRep_IsDead();
        GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AGenesisCharacter::Respawn, 5.0f, false);
    }
}

void AGenesisCharacter::OnRep_IsDead()
{
    if (bIsDead)
    {
        if (Controller) Controller->SetIgnoreMoveInput(true);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        BP_OnDeath();
    }
}

void AGenesisCharacter::Respawn()
{
    if (HasAuthority() && DeathController)
    {
        if (DeathController->GetPawn() == this)
        {
            DeathController->UnPossess();
        }
        if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
        {
            GM->RestartPlayer(DeathController);
        }
        Destroy();
    }
}

// --- MOVEMENT IMPLEMENTATION ---

void AGenesisCharacter::Move(const FInputActionValue& Value)
{
    if (bIsDead) return;

    FVector2D MovementVector = Value.Get<FVector2D>();
    DoMove(MovementVector.X, MovementVector.Y);
}

void AGenesisCharacter::Look(const FInputActionValue& Value)
{
    if (bIsDead) return; 
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    DoLook(LookAxisVector.X, LookAxisVector.Y);
}

// --- EXPOSED FUNCTIONS FOR BLUEPRINT & TOUCH INTERFACE ---

void AGenesisCharacter::DoMove(float Right, float Forward)
{
    if (Controller != nullptr && !bIsDead)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, Forward);
        AddMovementInput(RightDirection, Right);
    }
}

void AGenesisCharacter::DoLook(float Yaw, float Pitch)
{
    if (Controller != nullptr && !bIsDead)
    {
        AddControllerYawInput(Yaw);
        AddControllerPitchInput(Pitch);
    }
}

void AGenesisCharacter::DoJumpStart()
{
    if (!bIsDead) Jump();
}

void AGenesisCharacter::DoJumpEnd()
{
    if (!bIsDead) StopJumping();
}

// --- GAS HELPER ---

void AGenesisCharacter::AdjustStamina(float Amount)
{
    if (AbilitySystemComponent && AttributeSet)
    {
        float CurrentStamina = AbilitySystemComponent->GetNumericAttribute(UGenesisAttributeSet::GetStaminaAttribute());
        float MaxStamina = AbilitySystemComponent->GetNumericAttribute(UGenesisAttributeSet::GetMaxStaminaAttribute());
        float NewValue = FMath::Clamp(CurrentStamina + Amount, 0.0f, MaxStamina);
        AbilitySystemComponent->SetNumericAttributeBase(UGenesisAttributeSet::GetStaminaAttribute(), NewValue);
    }
}

UAbilitySystemComponent* AGenesisCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}