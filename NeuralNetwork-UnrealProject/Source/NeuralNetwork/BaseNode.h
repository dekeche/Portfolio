// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NeuralComponent.h"
#include "BaseOrganism.h"
#include "BaseNode.generated.h"


UCLASS()
class NEURALNETWORK_API ABaseNode : public AActor
{
	GENERATED_BODY()
private:

public:	
	// Sets default values for this actor's properties
	ABaseNode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "NodeProcess")
	void Reset();


	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NodeProcess")
	virtual void DoReset();
	//void DoReset_Implementation();


	UFUNCTION()
	void Process();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NodeProcess")
	virtual void DoProcess();
	//void DoProcess_Implementation();

	UFUNCTION(BlueprintCallable, Category = "NodeProcess")
	void LinkNeuron(UNeuralComponent* ref);
	UFUNCTION(BlueprintCallable, Category = "NodeProcess")
	void LinkNode(ABaseNode* ref);

	UFUNCTION()
	void TurnOn();

	UFUNCTION()
	void TurnOff();

	UFUNCTION(BlueprintCallable, Category = "NodeProcess")
	UNeuralComponent* GetNeural();

	UFUNCTION()
	void SetBase(float incoming, float calmDown, float outgoing, bool negative = false);

	UFUNCTION()
	void SetGene(FGene gene);
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UStaticMeshComponent* NodeObj;

 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UNeuralComponent* Neural;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UMaterial* OnMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Node")
	UMaterial* OffMaterial;

};
