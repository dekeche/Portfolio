// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "BaseOrganism.h"
#include "Environment.generated.h"

const int NumHerb = 20;
const int NumPlant = 50;
const int NumCarn = 0;
const int GenesPerTer = 5.0f;
const float MutationRate = 0.1f;

const int MaxHeight = 1600;
const int MaxWidth = 1600;

/**
 * 
 */
UCLASS()
class NEURALNETWORK_API AEnvironment : public AActor
{
	GENERATED_BODY()

public:
	AEnvironment();
	~AEnvironment();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Environment")
	static AEnvironment* GetRef();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Environment")
	void AddOrganism(ABaseOrganism* ref);
	void RemoveOrganism(ABaseOrganism*);
	TArray<ABaseOrganism*> GetOrganisms(EOrganismType);

	void HerbDeath(float, FGenome);
	FGenome HerbCreate();

	FGenome RandomGenome();
	FGene RandomGene();

	FGenome GetEvolvedGenome(EOrganismType type);
	FGene GetEvolvedGene(FGene,FGene);

	UFUNCTION(BlueprintCallable, Category = "Environment")
	void StartEvolution(TSubclassOf<ABaseOrganism> herb, TSubclassOf<ABaseOrganism> plant,
		TSubclassOf<ABaseOrganism> carn);

	void Evolve();
private:

	bool Destroying;
	static AEnvironment* evo;
	TArray<ABaseOrganism*> mPlants;
	TArray<ABaseOrganism*> mHerbs;
	TArray<ABaseOrganism*> mCarns;

	TMultiMap<float, FGenome> mGenerationHerb;
	TMultiMap<float, FGenome> mGenerationCarn;
	UWorld* mWorld;
	TSubclassOf<ABaseOrganism> mHerb;
	TSubclassOf<ABaseOrganism> mPlant;
	TSubclassOf<ABaseOrganism> mCarn;
};
