// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
class AEyeNode;
class ABaseNode;
class AOutNode;
struct FGene;

#include "GameFramework/Actor.h"
#include "BaseOrganism.h"
#include "HerbOrganism.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProcessNodes);

UCLASS()
class NEURALNETWORK_API AHerbOrganism : public ABaseOrganism
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHerbOrganism();

	void Setup(FGenome);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void DoOnHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mNodeBase;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mRightPlant;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mLeftPlant;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mUpPlant;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mRightHerb;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mLeftHerb;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mUpHerb;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mRightCarn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mLeftCarn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mUpCarn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mRightHidden;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mLeftHidden;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mUpHidden;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mDownHidden;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mRight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mLeft;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UChildActorComponent* mUp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	ABaseNode* mBase;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MovementSpeed")
	float movementMultiplier = 6.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	TArray<AOutNode*> mOut;

	DECLARE_EVENT(ABaseOrganism, FProcessNodes)
	FProcessNodes& OnProcess() { return ProcessData; };

private:

	FProcessNodes ProcessData;

	float mTime;
	FGenome mGenes;
};
