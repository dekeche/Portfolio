// Fill out your copyright notice in the Description page of Project Settings.

#include "NeuralNetwork.h"
#include "Environment.h"

AEnvironment* AEnvironment::evo = nullptr;
AEnvironment::AEnvironment()
{
	PrimaryActorTick.bCanEverTick = true;
	Destroying = false;
	evo = this;
};
AEnvironment::~AEnvironment()
{
	Destroying = true;
	if(evo == this)
		evo = nullptr;
}

void AEnvironment::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Destroying = true;

	while (mCarns.Num() != 0)
	{
		auto temp = mCarns[0];
		mCarns.Remove(temp);
		temp->Destroy();
	}
	while (mPlants.Num() != 0)
	{
		auto temp = mPlants[0];
		mPlants.Remove(temp);
		temp->Destroy();
	}
	while (mHerbs.Num() != 0)
	{
		auto temp = mHerbs[0];
		mHerbs.Remove(temp);
		temp->Destroy();
	}
};
AEnvironment* AEnvironment::GetRef()
{
	return evo;
};

void AEnvironment::Tick(float DeltaSeconds)
{
	
	if (mPlants.Num() == 0 || mHerbs.Num() == 0)
	{
		while (mCarns.Num() != 0)
		{
			auto temp = mCarns[0];
			mCarns.Remove(temp);
			temp->Destroy();
		}
		while (mPlants.Num() != 0)
		{
			auto temp = mPlants[0];
			mPlants.Remove(temp);
			temp->Destroy();
		}
		while (mHerbs.Num() != 0)
		{
			auto temp = mHerbs[0];
			mHerbs.Remove(temp);
			temp->Destroy();
		}
		Evolve();
	}
	
}

void AEnvironment::AddOrganism(ABaseOrganism* ref)
{
	if (!ref->IsActorBeingDestroyed())
	{
		switch (ref->GetType())
		{
		case EOrganismType::BO_PLANT:
			mPlants.Add(ref);
			break;
		case EOrganismType::BO_HERB:
			mHerbs.Add(ref);
			break;
		case EOrganismType::BO_CARN:
			mCarns.Add(ref);
			break;
		default:
			break;
		}
	}
};

void AEnvironment::RemoveOrganism(ABaseOrganism* ref)
{
	switch (ref->GetType())
	{
	case EOrganismType::BO_PLANT:
		mPlants.Remove(ref);
		break;
	case EOrganismType::BO_HERB:
		mHerbs.Remove(ref);
		break;
	case EOrganismType::BO_CARN:
		mCarns.Remove(ref);
		break;
	default:
		break;
	}
}
TArray<ABaseOrganism*> AEnvironment::GetOrganisms(EOrganismType type)
{
	switch (type)
	{
	case EOrganismType::BO_PLANT:
		return mPlants;
		break;
	case EOrganismType::BO_HERB:
		return mHerbs;
		break;
	case EOrganismType::BO_CARN:
		return mCarns;
		break;
	default:
		break;
	}
	TArray<ABaseOrganism*> temp;
	return temp;
};


void AEnvironment::HerbDeath(float key, FGenome value)
{
	mGenerationHerb.Add(key, value);
}

FGenome AEnvironment::HerbCreate()
{
	FGenome temp;
	if (mGenerationHerb.Num() == 0)
	{
		temp = RandomGenome();
	}
	else
	{
		temp = GetEvolvedGenome(EOrganismType::BO_HERB);
	}

	return temp;
};

FGenome AEnvironment::RandomGenome()
{
	FGenome temp;
	temp.EyeRightHerb = RandomGene();
	temp.EyeLeftHerb = RandomGene();
	temp.EyeUpHerb = RandomGene();

	temp.EyeRightCarn = RandomGene();
	temp.EyeLeftCarn = RandomGene();
	temp.EyeUpCarn = RandomGene();

	temp.EyeRightPlant = RandomGene();
	temp.EyeLeftPlant = RandomGene();
	temp.EyeUpPlant = RandomGene();


	temp.HiddenRight = RandomGene();
	temp.HiddenLeft = RandomGene();
	temp.HiddenUp = RandomGene();
	temp.HiddenDown = RandomGene();
	temp.HiddenDown.negative = true;

	temp.MoveRight = RandomGene();
	temp.MoveLeft = RandomGene();
	temp.MoveUp = RandomGene();
	return temp;
};
FGene AEnvironment::RandomGene()
{
	FGene temp;

	temp.incoming = ((float)rand() / (RAND_MAX));
	temp.calmDown = ((float)rand() / (RAND_MAX));
	temp.outgoing = ((float)rand() / (RAND_MAX));
	temp.negative = false;

	return temp;
};


FGenome AEnvironment::GetEvolvedGenome(EOrganismType type)
{
	TMultiMap<float, FGenome> base;
	switch (type)
	{
	case EOrganismType::BO_HERB:
		base = mGenerationHerb;
		break;
	case EOrganismType::BO_CARN:
		base = mGenerationCarn;
		break;
	default:
		break;
	}
	int size = base.Num();
	int perTer = GenesPerTer;
	int numTers = size / GenesPerTer;
	if (numTers == 0)
	{
		numTers = 0;
		perTer = size;
	}
	int factorialK = 0;
	for (int i = 1; i <= numTers; i++)
		factorialK += i;



	float Pull1 = ((float)rand() / (RAND_MAX));
	float Pull2 = ((float)rand() / (RAND_MAX));
	FGenome GeneSet1;
	FGenome GeneSet2;

	float Range = 0;
	float RTer = 0;
	float MinRange = 0;
	//float MaxRange = 0;
	auto iter = base.CreateIterator();
	bool found1 = false;
	bool found2 = false;
	for (int i = 0; i < numTers; i++)
	{
		Range = (float)(numTers - i) / factorialK;
		RTer = Range / perTer;
		//MaxRange = MinRange + Range;
		for (int k = 1; k <= perTer; k++)
		{

			if (MinRange + k*RTer > Pull1)
			{
				GeneSet1 = iter.Value();
				Pull1 = 2.0f;
				found1 = true;
			}
			if (MinRange + k*RTer > Pull2)
			{
				GeneSet2 = iter.Value();
				Pull2 = 2.0f;
				found2 = true;
			}
			if (Pull1 > 1.5f && Pull2 > 1.5f)
			{
				k = perTer;
				i = numTers;
			}
			++iter;
		}
		MinRange += Range;
	}
	if (!found1 || !found2)
	{
		UE_LOG(LogTemp, Log, TEXT("EvolutionError\n"))
		if (!found1)
			GeneSet1 = RandomGenome();
		if (!found2)
			GeneSet2 = RandomGenome();
	}
	FGenome Result;
	Result.EyeRightHerb = GetEvolvedGene(GeneSet1.EyeRightHerb,GeneSet2.EyeRightHerb);
	Result.EyeLeftHerb = GetEvolvedGene(GeneSet1.EyeLeftHerb, GeneSet2.EyeLeftHerb);
	Result.EyeUpHerb = GetEvolvedGene(GeneSet1.EyeUpHerb, GeneSet2.EyeUpHerb);

	Result.EyeRightCarn = GetEvolvedGene(GeneSet1.EyeRightCarn, GeneSet2.EyeRightCarn);
	Result.EyeLeftCarn = GetEvolvedGene(GeneSet1.EyeLeftCarn, GeneSet2.EyeLeftCarn);
	Result.EyeUpCarn = GetEvolvedGene(GeneSet1.EyeUpCarn, GeneSet2.EyeUpCarn);

	Result.EyeRightPlant = GetEvolvedGene(GeneSet1.EyeRightPlant, GeneSet2.EyeRightPlant);
	Result.EyeLeftPlant = GetEvolvedGene(GeneSet1.EyeLeftPlant, GeneSet2.EyeLeftPlant);
	Result.EyeUpPlant = GetEvolvedGene(GeneSet1.EyeUpPlant, GeneSet2.EyeUpPlant);


	Result.HiddenRight = GetEvolvedGene(GeneSet1.HiddenRight, GeneSet2.HiddenRight);
	Result.HiddenLeft = GetEvolvedGene(GeneSet1.HiddenLeft, GeneSet2.HiddenLeft);
	Result.HiddenUp = GetEvolvedGene(GeneSet1.HiddenUp, GeneSet2.HiddenUp);
	Result.HiddenDown = GetEvolvedGene(GeneSet1.HiddenDown, GeneSet2.HiddenDown);

	Result.MoveRight = GetEvolvedGene(GeneSet1.MoveRight, GeneSet2.MoveRight);
	Result.MoveLeft = GetEvolvedGene(GeneSet1.MoveLeft, GeneSet2.MoveLeft);
	Result.MoveUp = GetEvolvedGene(GeneSet1.MoveUp, GeneSet2.MoveUp);
	return Result;
};
FGene AEnvironment::GetEvolvedGene(FGene right, FGene left)
{
	MutationRate;
	float mutate = ((float)rand() / (RAND_MAX));
	if (mutate < MutationRate)
		return RandomGene();

	mutate = ((float)rand() / (RAND_MAX));
	if (mutate < 0.5f)
		return right;
	else
		return left;
};

void AEnvironment::StartEvolution(TSubclassOf<ABaseOrganism> herb, TSubclassOf<ABaseOrganism> plant,
	TSubclassOf<ABaseOrganism> carn)
{
	mWorld = GetWorld();

	mHerb = herb;
	mPlant = plant;
	mCarn = carn;
	Evolve();
};

void AEnvironment::Evolve()
{
	mGenerationHerb.KeySort([](float A, float B) {
		return A > B; // sort keys in reverse
	});
	for (int i = 0; i < NumPlant; i++)
	{
		float width;
		float height = rand() % MaxHeight;
		if (height < (MaxHeight / 3) || height >(2*MaxHeight / 3))
		{
			width = rand() % MaxWidth;
		}
		else
		{
			width = rand() % (2*MaxWidth/3);
			if (width > (MaxWidth / 3))
				width += (MaxWidth / 3);
		}
		FVector location(width, height, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		auto org = GetWorld()->SpawnActor<ABaseOrganism>(mPlant, location, rotation);
		if (org != nullptr)
		{
			if (!org->IsActorBeingDestroyed())
			{
				AddOrganism(org);
			}
		}
	}
	for (int i = 0; i < NumHerb; i++)
	{
		/*float width;
		float height = rand() % MaxHeight;
		if (height < (MaxHeight / 3) || height >(2 * MaxHeight / 3))
		{
			width = rand() % MaxWidth;
		}
		else
		{
			width = rand() % (2 * MaxWidth / 3);
			if (width > (MaxWidth / 3))
				width += (MaxWidth / 3);
		}*/
		FVector location(MaxHeight / 2, MaxWidth / 2, 0.0f);
		FRotator rotation(0.0f,0.0f,0.0f);
		auto org = GetWorld()->SpawnActor<ABaseOrganism>(mHerb, location, rotation);
		if(org != nullptr)
			AddOrganism(org);
	}
	for (int i = 0; i < NumCarn; i++)
	{
		FVector location(MaxHeight/2, MaxWidth/2, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		auto org = GetWorld()->SpawnActor<ABaseOrganism>(mCarn, location, rotation);
		if (org != nullptr)
			AddOrganism(org);
	}
	mGenerationHerb.Empty();
	mGenerationCarn.Empty();
};