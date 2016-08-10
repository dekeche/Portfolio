// Fill out your copyright notice in the Description page of Project Settings.

#include "NeuralNetwork.h"
#include "HerbOrganism.h"
#include "EyeNode.h"
#include "BaseNode.h"
#include "OutNode.h"
#include "Environment.h"


// Sets default values
AHerbOrganism::AHerbOrganism()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	type = EOrganismType::BO_HERB;
	
	mNodeBase = CreateDefaultSubobject<UChildActorComponent>(TEXT("BaseNode"));
	mNodeBase->AttachTo(Mesh);
	mNodeBase->SetChildActorClass(ABaseNode::StaticClass());

	mRightPlant = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightPlant"));
	mRightPlant->AttachTo(Mesh);
	mRightPlant->SetChildActorClass(AEyeNode::StaticClass());
	mLeftPlant = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftPlant"));
	mLeftPlant->AttachTo(Mesh);
	mLeftPlant->SetChildActorClass(AEyeNode::StaticClass());
	mUpPlant = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpPlant"));
	mUpPlant->AttachTo(Mesh);
	mUpPlant->SetChildActorClass(AEyeNode::StaticClass());

	mRightHerb = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightHerb"));
	mRightHerb->AttachTo(Mesh);
	mRightHerb->SetChildActorClass(AEyeNode::StaticClass());
	mLeftHerb = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftHerb"));
	mLeftHerb->AttachTo(Mesh);
	mLeftHerb->SetChildActorClass(AEyeNode::StaticClass());
	mUpHerb = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpHerb"));
	mUpHerb->AttachTo(Mesh);
	mUpHerb->SetChildActorClass(AEyeNode::StaticClass());

	mRightCarn = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightCarn"));
	mRightCarn->AttachTo(Mesh);
	mRightCarn->SetChildActorClass(AEyeNode::StaticClass());
	mLeftCarn = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftCarn"));
	mLeftCarn->AttachTo(Mesh);
	mLeftCarn->SetChildActorClass(AEyeNode::StaticClass());
	mUpCarn = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpCarn"));
	mUpCarn->AttachTo(Mesh);
	mUpCarn->SetChildActorClass(AEyeNode::StaticClass());

	mRightHidden = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightHidden"));
	mRightHidden->AttachTo(Mesh);
	mRightHidden->SetChildActorClass(ABaseNode::StaticClass());
	mLeftHidden = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftHidden"));
	mLeftHidden->AttachTo(Mesh);
	mLeftHidden->SetChildActorClass(ABaseNode::StaticClass());
	mUpHidden = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpHidden"));
	mUpHidden->AttachTo(Mesh);
	mUpHidden->SetChildActorClass(ABaseNode::StaticClass());

	mDownHidden = CreateDefaultSubobject<UChildActorComponent>(TEXT("DownHidden"));
	mDownHidden->AttachTo(Mesh);
	mDownHidden->SetChildActorClass(ABaseNode::StaticClass());

	mRight = CreateDefaultSubobject<UChildActorComponent>(TEXT("Right"));
	mRight->AttachTo(Mesh);
	mRight->SetChildActorClass(AOutNode::StaticClass());
	mLeft = CreateDefaultSubobject<UChildActorComponent>(TEXT("Left"));
	mLeft->AttachTo(Mesh);
	mLeft->SetChildActorClass(AOutNode::StaticClass());
	mUp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Up"));
	mUp->AttachTo(Mesh);
	mUp->SetChildActorClass(AOutNode::StaticClass());
}

void AHerbOrganism::Setup(FGenome genome)
{
	mGenes = genome;
	mTime = 0.0f;
	//base -> eye -> hidden nodes -> out
	ABaseNode* TempEyePlantRight; ABaseNode* TempEyePlantLeft; ABaseNode* TempEyePlantUp;
	ABaseNode* TempCarnEyeRight; ABaseNode* TempCarnEyeLeft; ABaseNode* TempCarnEyeUp;
	ABaseNode* TempHerbEyeRight; ABaseNode* TempHerbEyeLeft; ABaseNode* TempHerbEyeUp;
	ABaseNode* TempHiddenRight; ABaseNode* TempHiddenLeft; ABaseNode* TempHiddenUp; ABaseNode* TempHiddenDown;
	ABaseNode* TempOutRight; ABaseNode* TempOutLeft; ABaseNode* TempOutUp;
	mBase = (ABaseNode*)mNodeBase->ChildActor;

	mBase->SetBase(0.0f,0.0f,1.0f);

	TempOutRight = (ABaseNode*)mRight->ChildActor;
	TempOutRight->SetGene(genome.MoveRight);
	TempOutLeft = (ABaseNode*)mLeft->ChildActor;
	TempOutLeft->SetGene(genome.MoveLeft);
	TempOutUp = (ABaseNode*)mUp->ChildActor;
	TempOutUp->SetGene(genome.MoveUp);

	TempHiddenDown = (ABaseNode*)mDownHidden->ChildActor;
	TempHiddenDown->SetGene(genome.HiddenDown);
	TempHiddenRight = (ABaseNode*)mRightHidden->ChildActor;
	TempHiddenRight->SetGene(genome.HiddenRight);
	TempHiddenLeft = (ABaseNode*)mLeftHidden->ChildActor;
	TempHiddenLeft->SetGene(genome.HiddenLeft);
	TempHiddenUp = (ABaseNode*)mUpHidden->ChildActor;
	TempHiddenUp->SetGene(genome.HiddenUp);

	TempHerbEyeRight = (ABaseNode*)mRightHerb->ChildActor;
	TempHerbEyeRight->SetGene(genome.EyeRightHerb);
	TempHerbEyeLeft = (ABaseNode*)mLeftHerb->ChildActor;
	TempHerbEyeLeft->SetGene(genome.EyeLeftHerb);
	TempHerbEyeUp = (ABaseNode*)mUpHerb->ChildActor;
	TempHerbEyeUp->SetGene(genome.EyeUpHerb);

	TempCarnEyeRight = (ABaseNode*)mRightCarn->ChildActor;
	TempCarnEyeRight->SetGene(genome.EyeRightCarn);
	TempCarnEyeLeft = (ABaseNode*)mLeftCarn->ChildActor;
	TempCarnEyeLeft->SetGene(genome.EyeLeftCarn);
	TempCarnEyeUp = (ABaseNode*)mUpCarn->ChildActor;
	TempCarnEyeUp->SetGene(genome.EyeUpCarn);

	TempEyePlantRight = (ABaseNode*)mRightPlant->ChildActor;
	TempEyePlantRight->SetGene(genome.EyeRightPlant);
	TempEyePlantLeft = (ABaseNode*)mLeftPlant->ChildActor;
	TempEyePlantLeft->SetGene(genome.EyeLeftPlant);
	TempEyePlantUp = (ABaseNode*)mUpPlant->ChildActor;
	TempEyePlantUp->SetGene(genome.EyeUpPlant);

	mOut.Add((AOutNode*)TempOutRight);
	mOut.Add((AOutNode*)TempOutLeft);
	mOut.Add((AOutNode*)TempOutUp);


	((AEyeNode*)(TempEyePlantRight))->setup(this, 0.0f,600.0f,0.4f,0.8f, true, false, EOrganismType::BO_PLANT);
	((AEyeNode*)(TempEyePlantLeft))->setup(this, 0.0f, 600.0f, 0.4f, 0.8f, false, true, EOrganismType::BO_PLANT);
	((AEyeNode*)(TempEyePlantUp))->setup(this, 0.0f, 600.0f, 0.8f, 1.0f, true, true, EOrganismType::BO_PLANT);

	((AEyeNode*)(TempCarnEyeRight))->setup(this, 0.0f, 600.0f, 0.4f, 0.8f, true, false, EOrganismType::BO_CARN);
	((AEyeNode*)(TempCarnEyeLeft))->setup(this, 0.0f, 600.0f, 0.4f, 0.8f, false, true, EOrganismType::BO_CARN);
	((AEyeNode*)(TempCarnEyeUp))->setup(this, 0.0f, 600.0f, 0.8f, 1.0f, true, true, EOrganismType::BO_CARN);

	((AEyeNode*)(TempHerbEyeRight))->setup(this, 0.0f, 600.0f, 0.4f, 0.8f, true, false, EOrganismType::BO_HERB);
	((AEyeNode*)(TempHerbEyeLeft))->setup(this, 0.0f, 600.0f, 0.4f, 0.8f, false, true, EOrganismType::BO_HERB);
	((AEyeNode*)(TempHerbEyeUp))->setup(this, 0.0f, 600.0f, 0.8f, 1.0f, true, true, EOrganismType::BO_HERB);


	((AOutNode*)TempOutRight)->SetVector(FVector(30.0f, -15.0f,0.0f));
	((AOutNode*)TempOutLeft)->SetVector(FVector(30.0f, 15.0f, 0.0f));
	((AOutNode*)TempOutUp)->SetVector(FVector(50.0f, 0.0f, 0.0f));


	ProcessData.AddUFunction(TempEyePlantRight, "Reset");
	ProcessData.AddUFunction(TempEyePlantLeft, "Reset");
	ProcessData.AddUFunction(TempEyePlantUp, "Reset");
	ProcessData.AddUFunction(TempCarnEyeRight, "Reset");
	ProcessData.AddUFunction(TempCarnEyeLeft, "Reset");
	ProcessData.AddUFunction(TempCarnEyeUp, "Reset");
	ProcessData.AddUFunction(TempHerbEyeRight, "Reset");
	ProcessData.AddUFunction(TempHerbEyeLeft, "Reset");
	ProcessData.AddUFunction(TempHerbEyeUp, "Reset");
	ProcessData.AddUFunction(TempHiddenRight, "Reset");
	ProcessData.AddUFunction(TempHiddenLeft, "Reset");
	ProcessData.AddUFunction(TempHiddenUp, "Reset");
	ProcessData.AddUFunction(TempHiddenDown, "Reset");
	ProcessData.AddUFunction(TempOutRight, "Reset");
	ProcessData.AddUFunction(TempOutLeft, "Reset");
	ProcessData.AddUFunction(TempOutUp, "Reset");
	ProcessData.AddUFunction(mBase, "Reset");

	mBase->LinkNode(TempEyePlantRight);
	TempEyePlantRight->LinkNode(TempHiddenRight);
	TempEyePlantRight->LinkNode(TempHiddenUp);
	TempEyePlantRight->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempEyePlantLeft);
	TempEyePlantLeft->LinkNode(TempHiddenLeft);
	TempEyePlantLeft->LinkNode(TempHiddenUp);
	TempEyePlantLeft->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempEyePlantUp);
	TempEyePlantUp->LinkNode(TempHiddenUp);
	TempEyePlantUp->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempCarnEyeRight);
	TempCarnEyeRight->LinkNode(TempHiddenRight);
	TempCarnEyeRight->LinkNode(TempHiddenUp);
	TempCarnEyeRight->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempCarnEyeLeft);
	TempCarnEyeLeft->LinkNode(TempHiddenLeft);
	TempCarnEyeLeft->LinkNode(TempHiddenUp);
	TempCarnEyeLeft->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempCarnEyeUp);
	TempCarnEyeUp->LinkNode(TempHiddenUp);
	TempCarnEyeUp->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempHerbEyeRight);
	TempHerbEyeRight->LinkNode(TempHiddenRight);
	TempHerbEyeRight->LinkNode(TempHiddenUp);
	TempHerbEyeRight->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempHerbEyeLeft);
	TempHerbEyeLeft->LinkNode(TempHiddenLeft);
	TempHerbEyeLeft->LinkNode(TempHiddenUp);
	TempHerbEyeLeft->LinkNode(TempHiddenDown);

	mBase->LinkNode(TempHerbEyeUp);
	TempHerbEyeUp->LinkNode(TempHiddenUp);
	TempHerbEyeUp->LinkNode(TempHiddenDown);

	TempHiddenRight->LinkNode(TempOutRight);
	TempHiddenLeft->LinkNode(TempOutLeft);
	TempHiddenUp->LinkNode(TempOutUp);
	TempHiddenDown->LinkNode(TempOutUp);
};
// Called when the game starts or when spawned
void AHerbOrganism::BeginPlay()
{
	Super::BeginPlay();

	auto env = AEnvironment::GetRef();

	Setup(env->HerbCreate());

	ProcessData.Broadcast();
	mBase->Process();

}

void AHerbOrganism::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	mTime += (EnergyCurrent / EnergyDrain);
	AEnvironment::GetRef()->HerbDeath(mTime, mGenes);
	Super::EndPlay(EndPlayReason);
};
// Called every frame
void AHerbOrganism::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	ProcessData.Broadcast();
	mBase->Process();
	EnergyCurrent -= EnergyDrain*DeltaTime;
	mTime += DeltaTime;

	FVector vector(0,0,0);
	for (int i = 0; i < mOut.Num(); i++)
	{
		vector += mOut[i]->GetMovement();
	}
	FVector Axis;
	float Angle;
	GetActorRotation().Quaternion().ToAxisAndAngle(Axis,Angle);
	Angle = FMath::RadiansToDegrees(Angle);

	vector = vector.RotateAngleAxis(Angle,Axis);
	vector *= (DeltaTime* movementMultiplier);
	//GetActorRotation().RotateVector(vector);
	auto loc = GetActorLocation();
	float X = vector.X;
	float Y = vector.Y;
	float Z = vector.Z;
	loc += vector;
	if(loc.X < 0)
	{
		loc.X += MaxWidth;
	}
	else if (loc.X > MaxWidth)
	{
		loc.X -= MaxWidth;
	}
	if (loc.Y < 0)
	{
		loc.Y += MaxHeight;
	}
	else if (loc.Y > MaxHeight)
	{
		loc.Y -= MaxHeight;
	}
	SetActorLocation(loc);
	SetActorRotation(vector.Rotation());
	if (EnergyCurrent <= 0)
	{
		Destroy();
	}
}


void AHerbOrganism::DoOnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(ABaseOrganism::StaticClass()))
	{
		auto object = Cast<ABaseOrganism>(OtherActor);
		switch (object->GetType())
		{
		case EOrganismType::BO_PLANT:
			AddEnergy(object->GetEnergy());
			OtherActor->Destroy();
			break;
		case EOrganismType::BO_HERB:
			break;
		case EOrganismType::BO_CARN:
			break;
		default:
			break;
		}
	}
};