// Copyright GenesisTeam


#include "Character/GenesisEnemyCharacter.h"

#include "AbilitySystem/GenesisAbilitySystemComponent.h"
#include "AbilitySystem/GenesisAttributeSet.h"

AGenesisEnemyCharacter::AGenesisEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGenesisAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	
	AttributeSet = CreateDefaultSubobject<UGenesisAttributeSet>("AttributeSet");
}
