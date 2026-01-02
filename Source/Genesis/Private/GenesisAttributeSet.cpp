
#include "GenesisAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GenesisCharacter.h"
#include "GameplayEffectExtension.h"

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

void UGenesisAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Clamp Health
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        // Check Death
        if (GetHealth() <= 0.0f)
        {
            // Cast to GenesisCharacter and call Die()
            if (AGenesisCharacter* Avatar = Cast<AGenesisCharacter>(Data.Target.GetAvatarActor()))
            {
                Avatar->Die();
            }
        }
    }
}