// Copyright GenesisTeam


#include "UI/Widgets/GenesisUserWidget.h"

void UGenesisUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
