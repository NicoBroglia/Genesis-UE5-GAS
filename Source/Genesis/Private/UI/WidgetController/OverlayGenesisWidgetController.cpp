// Copyright GenesisTeam


#include "UI/WidgetController/OverlayGenesisWidgetController.h"

#include "AbilitySystem/GenesisAttributeSet.h"

void UOverlayGenesisWidgetController::BroadCastInitialValues()
{
	const UGenesisAttributeSet* GenesisAttributeSet = CastChecked<UGenesisAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(GenesisAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(GenesisAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(GenesisAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(GenesisAttributeSet->GetMaxMana());
}

void UOverlayGenesisWidgetController::BindCallbacksToDependencies()
{
	const UGenesisAttributeSet* GenesisAttributeSet = CastChecked<UGenesisAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GenesisAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayGenesisWidgetController::HealthChanged); 
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GenesisAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayGenesisWidgetController::MaxHealthChanged); 
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GenesisAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayGenesisWidgetController::ManaChanged); 
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GenesisAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayGenesisWidgetController::MaxManaChanged); 
}

void UOverlayGenesisWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayGenesisWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayGenesisWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayGenesisWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
