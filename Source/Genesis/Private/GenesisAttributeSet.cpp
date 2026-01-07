#include "GenesisAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GenesisCharacter.h"
#include "GameplayEffectExtension.h"

UGenesisAttributeSet::UGenesisAttributeSet()
{
	// Default constructor
}

// 1. Setup Replication Rules
void UGenesisAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register variables for replication (Server -> Client)
	DOREPLIFETIME_CONDITION_NOTIFY(UGenesisAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGenesisAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGenesisAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGenesisAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

// 2. OnRep Functions (Called on Client when Server updates a value)
void UGenesisAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenesisAttributeSet, Health, OldHealth);
}

void UGenesisAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenesisAttributeSet, MaxHealth, OldMaxHealth);
}

void UGenesisAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenesisAttributeSet, Stamina, OldStamina);
}

void UGenesisAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGenesisAttributeSet, MaxStamina, OldMaxStamina);
}

// 3. Logic Handling (Clamping & Death)
void UGenesisAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// --- LOGIC FOR HEALTH ---
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// 1. Clamp Health so it can't be negative or higher than Max
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		// 2. Check for Death (0 HP)
		if (GetHealth() <= 0.0f)
		{
			// Find the Character and kill them
			if (AGenesisCharacter* Avatar = Cast<AGenesisCharacter>(Data.Target.GetAvatarActor()))
			{
				Avatar->Die();
			}
		}
	}

	// --- LOGIC FOR STAMINA ---
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		// Clamp Stamina so it doesn't go below 0 or above Max
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
}