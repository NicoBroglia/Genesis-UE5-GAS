// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GenesisCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UGenesisAttributeSet;

UCLASS(config = Game)
class AGenesisCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AGenesisCharacter();

    // --- REPLICATION OVERRIDES ---
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // Server-side initialization
    virtual void PossessedBy(AController* NewController) override;
    
    // Client-side initialization
    virtual void OnRep_PlayerState() override; 

    // --- COMPONENTS ---

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // --- GAS INTERFACE ---
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY()
    UGenesisAttributeSet* AttributeSet;

    // --- NEW: HELPER FUNCTIONS FOR BLUEPRINT LOGIC ---

    /** Directly modifies stamina. Use negative values to drain, positive to regen. */
    UFUNCTION(BlueprintCallable, Category = "Genesis|GAS")
    void AdjustStamina(float Amount);

    /** Wrapper for DoMove logic to be used by Touch Interface / BP */
    UFUNCTION(BlueprintCallable, Category = "Genesis|Movement")
    void DoMove(float Right, float Forward);

    /** Wrapper for DoLook logic to be used by Touch Interface / BP */
    UFUNCTION(BlueprintCallable, Category = "Genesis|Movement")
    void DoLook(float Yaw, float Pitch);

    UFUNCTION(BlueprintCallable, Category = "Genesis|Movement")
    void DoJumpStart();

    UFUNCTION(BlueprintCallable, Category = "Genesis|Movement")
    void DoJumpEnd();

    // --- DEATH SYSTEM ---

    UPROPERTY(ReplicatedUsing = OnRep_IsDead, BlueprintReadOnly, Category = "Genesis|State")
    bool bIsDead;

    UFUNCTION()
    void OnRep_IsDead();

    virtual void Die();

    UFUNCTION(BlueprintImplementableEvent, Category = "Genesis|Combat")
    void BP_OnDeath();

protected:
    UPROPERTY()
    AController* DeathController; 

    FTimerHandle TimerHandle_Respawn;
    void Respawn();

    // --- INPUT ---

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* LookAction;

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

public:
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};