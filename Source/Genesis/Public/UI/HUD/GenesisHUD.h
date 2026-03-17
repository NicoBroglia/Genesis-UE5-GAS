// Copyright GenesisTeam

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GenesisHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayGenesisWidgetController;
class UGenesisUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class GENESIS_API AGenesisHUD : public AHUD
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TObjectPtr<UGenesisUserWidget> OverlayWidget;
	
	UOverlayGenesisWidgetController* GetOverlayWidgetController(const FWidgetControllerParams WCParams);
	
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
protected:

private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGenesisUserWidget> OverlayWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayGenesisWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayGenesisWidgetController> OverlayWidgetControllerClass;
};
