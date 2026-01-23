// Copyright GenesisTeam

#pragma once

#include "CoreMinimal.h"
//#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "GenesisMainCharacterController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class GENESIS_API AGenesisMainCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGenesisMainCharacterController();
protected:
	virtual void BeginPlay() override;
	
private:	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext>GenesisCharacterContext;
};
