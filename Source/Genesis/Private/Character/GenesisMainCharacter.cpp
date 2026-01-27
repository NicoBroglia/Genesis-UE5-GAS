// Copyright GenesisTeam


#include "Character/GenesisMainCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AGenesisMainCharacter::AGenesisMainCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); 
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
