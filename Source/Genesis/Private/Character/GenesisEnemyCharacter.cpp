// Copyright GenesisTeam


#include "Character/GenesisEnemyCharacter.h"

#include "AbilitySystem/GenesisAbilitySystemComponent.h"
#include "AbilitySystem/GenesisAttributeSet.h"

AGenesisEnemyCharacter::AGenesisEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGenesisAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UGenesisAttributeSet>("AttributeSet");
}

void AGenesisEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}
