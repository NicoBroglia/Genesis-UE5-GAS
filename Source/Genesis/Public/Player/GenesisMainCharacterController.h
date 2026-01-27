// Copyright GenesisTeam

#pragma once

#include "CoreMinimal.h"
//#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "GenesisMainCharacterController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

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
	virtual void SetupInputComponent() override;
	
private:	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext>GenesisCharacterContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	
	void Move(const FInputActionValue& InputActionValue);
	
	void Look(const FInputActionValue& InputActionValue);
};
