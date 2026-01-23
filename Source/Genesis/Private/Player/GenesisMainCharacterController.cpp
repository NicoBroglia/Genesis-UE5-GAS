// Copyright GenesisTeam


#include "Player/GenesisMainCharacterController.h"

#include "EnhancedInputSubsystems.h"

AGenesisMainCharacterController::AGenesisMainCharacterController()
{
	bReplicates = true;
}

void AGenesisMainCharacterController::BeginPlay()
{
	Super::BeginPlay();
	check(GenesisCharacterContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(GenesisCharacterContext, 0);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
