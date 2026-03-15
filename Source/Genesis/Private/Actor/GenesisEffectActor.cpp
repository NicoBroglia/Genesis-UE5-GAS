// Copyright GenesisTeam


#include "Actor/GenesisEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/GenesisAttributeSet.h"

AGenesisEffectActor::AGenesisEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AGenesisEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: here should be a gameplay effect. for now using const_cast
	if (IAbilitySystemInterface* ASCInterface =  Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UGenesisAttributeSet* GenesisAttributeSet = Cast<UGenesisAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UGenesisAttributeSet::StaticClass()));
			
		UGenesisAttributeSet* MutableGenesisAttributeSet = const_cast<UGenesisAttributeSet*>(GenesisAttributeSet);
		MutableGenesisAttributeSet->SetHealth(GenesisAttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void AGenesisEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AGenesisEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGenesisEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AGenesisEffectActor::EndOverlap);
}


