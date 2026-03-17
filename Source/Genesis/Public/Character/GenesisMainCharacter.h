// Copyright GenesisTeam

#pragma once

#include "CoreMinimal.h"
#include "Character/GenesisCharacterBase.h"
#include "GenesisMainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class GENESIS_API AGenesisMainCharacter : public AGenesisCharacterBase
{
	GENERATED_BODY()
public:
		AGenesisMainCharacter();
		virtual void PossessedBy(AController* NewController) override;
		virtual void OnRep_PlayerState() override;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundAcceleration = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundDeceleration = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundFriction = 6.0f;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	
private:
	void InitAbilityActorInfo();
};
