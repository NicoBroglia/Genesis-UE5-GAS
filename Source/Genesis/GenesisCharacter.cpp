// Copyright Epic Games, Inc. All Rights Reserved.

#include "GenesisCharacter.h"
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
#include "Net/UnrealNetwork.h" // Required for DOREPLIFETIME

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

    // GAS Setup - Note: Replication Mode 'Mixed' is generally better for Player Characters
    // so Gameplay Cues work reliably for the owner, but 'Minimal' is acceptable for prototypes.
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UGenesisAttributeSet>(TEXT("AttributeSet"));
    
    bIsDead = false;
}

// 1. REPLICATION SETUP
void AGenesisCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate the Death State to everyone
    DOREPLIFETIME(AGenesisCharacter, bIsDead);
}

// 2. ASC INITIALIZATION (The Correct Way)
void AGenesisCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // SERVER Init
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }

    // Add Default Abilities or Effects here if needed...
}

void AGenesisCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // CLIENT Init
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

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

// 3. ROBUST DEATH SYSTEM
void AGenesisCharacter::Die()
{
    // Ensure we only run this once on the Server
    if (HasAuthority() && !bIsDead)
    {
        bIsDead = true;
        
        // Save controller for respawn logic
        DeathController = GetController();

        // Server must call OnRep manually for itself
        OnRep_IsDead();

        // Start Respawn Timer
        GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &AGenesisCharacter::Respawn, 5.0f, false);
    }
}

void AGenesisCharacter::OnRep_IsDead()
{
    if (bIsDead)
    {
        // 1. Disable Input
        if (Controller) Controller->SetIgnoreMoveInput(true);
        
        // 2. Disable Collision
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
        // 3. Trigger Visuals (Ragdoll, etc) in BP
        BP_OnDeath();
    }
}

void AGenesisCharacter::Respawn()
{
    if (HasAuthority() && DeathController)
    {
        // Detach controller from dead body
        if (DeathController->GetPawn() == this)
        {
            DeathController->UnPossess();
        }

        // Restart
        if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
        {
            GM->RestartPlayer(DeathController);
        }

        Destroy();
    }
}

// --- MOVEMENT ---

void AGenesisCharacter::Move(const FInputActionValue& Value)
{
    if (bIsDead) return; // Prevent movement if dead

    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AGenesisCharacter::Look(const FInputActionValue& Value)
{
    if (bIsDead) return; 

    FVector2D LookAxisVector = Value.Get<FVector2D>();
    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

UAbilitySystemComponent* AGenesisCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}