// Copyright GenesisTeam


#include "UI/WidgetController/GenesisWidgetController.h"

void UGenesisWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UGenesisWidgetController::BroadCastInitialValues()
{
	
}

void UGenesisWidgetController::BindCallbacksToDependencies()
{
	
}
