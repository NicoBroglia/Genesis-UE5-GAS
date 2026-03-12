// Copyright GenesisTeam


#include "Player/GenesisPlayerState.h"

#include "AbilitySystem/GenesisAbilitySystemComponent.h"
#include "AbilitySystem/GenesisAttributeSet.h"
#include "AbilitySystem/GenesisAttributeSet.h"

AGenesisPlayerState::AGenesisPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGenesisAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	
	AttributeSet = CreateDefaultSubobject<UGenesisAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AGenesisPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
