// Copyright GenesisTeam

#include "Character/GenesisCharacterBase.h"

#include "AbilitySystemComponent.h"

AGenesisCharacterBase::AGenesisCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AGenesisCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGenesisCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
