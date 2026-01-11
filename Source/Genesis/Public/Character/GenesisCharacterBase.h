//Copyright GenesisTeam
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h" 
#include "GenesisCharacterBase.generated.h"


UCLASS(Abstract)
class GENESIS_API AGenesisCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGenesisCharacterBase();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
