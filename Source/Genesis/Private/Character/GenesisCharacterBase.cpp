// Copyright GenesisTeam

#include "Character/GenesisCharacterBase.h"

AGenesisCharacterBase::AGenesisCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGenesisCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
