
#include "GenesisAttributeSet.h"
#include "Net/UnrealNetwork.h"

UGenesisAttributeSet::UGenesisAttributeSet()
{
}

void UGenesisAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register variables for replication
	DOREPLIFETIME_CONDITION_NOTIFY(UGenesisAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGenesisAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UGenesisAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenesisAttributeSet, Health, OldHealth);
}

void UGenesisAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenesisAttributeSet, MaxHealth, OldMaxHealth);
}