// Copyright GenesisTeam

#include "Character/GenesisCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "Player/GenesisPlayerState.h"

AGenesisCharacterBase::AGenesisCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGenesisCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//init ability actor info for the server
	InitAbilityActorInfo();
}

void AGenesisCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the client
	InitAbilityActorInfo();
}

UAbilitySystemComponent* AGenesisCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGenesisCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGenesisCharacterBase::InitAbilityActorInfo()
{
	AGenesisPlayerState* GenesisPlayerState = GetPlayerState<AGenesisPlayerState>();
	check( GenesisPlayerState );
	GenesisPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(GenesisPlayerState, this);
	AbilitySystemComponent = GenesisPlayerState->GetAbilitySystemComponent();
	AttributeSet = GenesisPlayerState->GetAttributeSet();
}
