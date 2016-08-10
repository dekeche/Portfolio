
#include "NeuralNetwork.h"
#include "Environment.h"
#include "BaseOrganism.h"


ABaseOrganism::ABaseOrganism()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	this->RootComponent = SceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->AttachParent = SceneRoot;

	FScriptDelegate del;
	del.BindUFunction(this, "OnHit");
	Mesh->OnComponentBeginOverlap.Add(del);

	type = EOrganismType::BO_PLANT;
};

void ABaseOrganism::BeginPlay()
{
	EnergyCurrent = EnergyMax;
};
void ABaseOrganism::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AEnvironment::GetRef()->RemoveOrganism(this);
	Super::EndPlay(EndPlayReason);
};

void ABaseOrganism::OnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->IsActorBeingDestroyed() && !this->IsActorBeingDestroyed())
	{
		DoOnHit(OtherActor, OtherComp, NormalImpulse, Hit);
	}
}

void ABaseOrganism::DoOnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

};
