// Copyright GenesisTeam


#include "Character/GenesisMainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AGenesisMainCharacter::AGenesisMainCharacter()
{
	// Configure character movement for Souls-like feel
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f); // Fast enough to feel responsive, slow enough to see the turn
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	
	// Physics and Weight
	GetCharacterMovement()->GravityScale = 1.8f; 
	GetCharacterMovement()->MaxAcceleration = GroundAcceleration; 
	GetCharacterMovement()->BrakingDecelerationWalking = GroundDeceleration; 
	GetCharacterMovement()->GroundFriction = GroundFriction;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true; // Essential for precise stop control
	
	// Speed settings
	GetCharacterMovement()->MaxWalkSpeed = 400.0f; 
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.1f; 
	
	// Collision / Steps
	GetCharacterMovement()->MaxStepHeight = 35.0f; 
	GetCharacterMovement()->SetWalkableFloorAngle(44.0f); 
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 320.0f; // Tight Souls-style camera
	CameraBoom->SocketOffset = FVector(0.0f, 40.0f, 60.0f); // Slight "over the shoulder" offset
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.0f; // Heavier camera follow
	CameraBoom->CameraLagMaxDistance = 50.0f;
	CameraBoom->bDoCollisionTest = true; // Essential for the Souls feel in corridors
	CameraBoom->ProbeSize = 10.0f; // Prevents the camera from jumping on small objects

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}
