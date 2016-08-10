// Fill out your copyright notice in the Description page of Project Settings.

#include "NeuralNetwork.h"
#include "BaseNode.h"


// Sets default values
ABaseNode::ABaseNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	this->RootComponent = SceneRoot;

	NodeObj = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Node"));
	NodeObj->AttachTo(RootComponent);
	Neural = CreateDefaultSubobject<UNeuralComponent>(TEXT("Neural"));

}

// Called when the game starts or when spawned
void ABaseNode::BeginPlay()
{
	Super::BeginPlay();
	FProcessEvent temp;
	temp.BindUFunction(this, "Process");
	Neural->LinkProcessing(temp);
}

// Called every frame
void ABaseNode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void ABaseNode::Reset()
{
	Neural->Process();
	TurnOff();
	DoReset();
};

void ABaseNode::DoReset()
{
	
};
void ABaseNode::Process()
{
	TurnOn();
	DoProcess();
};

void ABaseNode::DoProcess()
{
	Neural->OutgoingBase();
};
void ABaseNode::LinkNeuron(UNeuralComponent* ref)
{
	Neural->LinkIncoming(ref);
};
void ABaseNode::LinkNode(ABaseNode* ref)
{
	LinkNeuron(ref->Neural);
};
void ABaseNode::TurnOn()
{
	NodeObj->SetMaterial(0,OnMaterial);
};
void ABaseNode::TurnOff()
{
	NodeObj->SetMaterial(0, OffMaterial);
};
UNeuralComponent* ABaseNode::GetNeural()
{
	return Neural;
};


void ABaseNode::SetBase(float incoming, float calmDown, float outgoing, bool negative)
{
	Neural->SetFireValues(incoming, calmDown, outgoing, negative);
};
void ABaseNode::SetGene(FGene gene)
{
	Neural->SetFireValues(gene.incoming,gene.calmDown,gene.outgoing,gene.negative);
};