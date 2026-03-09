// Copyright GenesisTeam

#pragma once

#include "CoreMinimal.h"
#include "Character/GenesisCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "GenesisEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GENESIS_API AGenesisEnemyCharacter : public AGenesisCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
};
