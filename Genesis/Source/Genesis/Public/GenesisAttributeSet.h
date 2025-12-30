#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GenesisAttributeSet.generated.h"

// Macros to define getters and setters automatically (Required by GAS)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GENESIS_API UGenesisAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UGenesisAttributeSet();

	// Required for multiplayer replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --- HEALTH ATTRIBUTE ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGenesisAttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	// --- MAX HEALTH ATTRIBUTE ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGenesisAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};