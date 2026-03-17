// Copyright GenesisTeam


#include "Player/GenesisMainCharacterController.h"

#include "EnhancedInputSubsystems.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

AGenesisMainCharacterController::AGenesisMainCharacterController()
{
	bReplicates = true;
}

void AGenesisMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

	//if (IsLocalPlayerController())
	//{
		check(GenesisCharacterContext);
		
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(GenesisCharacterContext, 0);
		}
		
		bShowMouseCursor = false; 
	    
		FInputModeGameOnly InputModeData;
		SetInputMode(InputModeData);

		if (PlayerCameraManager)
		{
			PlayerCameraManager->ViewPitchMin = PitchMin;
			PlayerCameraManager->ViewPitchMax = PitchMax;
			PlayerCameraManager->ViewYawMin = YawMin;
			PlayerCameraManager->ViewYawMax = YawMax;
		}
	}
//}

void AGenesisMainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGenesisMainCharacterController::Move);
        
		// Bind Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGenesisMainCharacterController::Look);

		// Bind Mouse Look (re-using the same Look function)
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGenesisMainCharacterController::Look);
	}
	
}

void AGenesisMainCharacterController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>() * LookSensitivity;

	// Apply Inversion Toggles
	const float YawValue = bInvertYaw ? -LookAxisVector.X : LookAxisVector.X;
	const float PitchValue = bInvertPitch ? -LookAxisVector.Y : LookAxisVector.Y;

	// Add Yaw (Left/Right) and Pitch (Up/Down) input to the Controller
	AddYawInput(YawValue);
	AddPitchInput(PitchValue);
}

void AGenesisMainCharacterController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
    
	// Get the rotation of the CONTROLLER (which matches the Camera/SpringArm)
	// We only care about Yaw (horizontal rotation) for movement
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Calculate Forward and Right based on where the camera is looking
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}


