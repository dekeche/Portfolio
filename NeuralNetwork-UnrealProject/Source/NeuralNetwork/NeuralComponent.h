// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "NeuralComponent.generated.h"


UDELEGATE(BlueprintType, Category = Delegate)
DECLARE_DYNAMIC_DELEGATE(FProcessEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEURALNETWORK_API UNeuralComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Neural")
		float IncomingFireTrigger;
	UPROPERTY(EditDefaultsOnly, Category = "Neural")
		float BaseCharge;
	UPROPERTY(EditDefaultsOnly, Category = "Neural")
		float CalmDownRate;
	UPROPERTY(EditDefaultsOnly, Category = "Neural")
		float OutgoingFire;
public:	
public:
	// Sets default values for this component's properties
	UNeuralComponent();

	UFUNCTION(BlueprintCallable, Category = "Neural")
	void SetFireValues(float incoming, float calmDown, float outgoing, bool negative = false);
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//setup
	UFUNCTION(BlueprintCallable, Category = "Neural")
	void Process();

	UFUNCTION(BlueprintCallable, Category = "Neural")
	void LinkIncoming(UNeuralComponent* ref);

	UFUNCTION(BlueprintCallable, Category = "Neural")
	void Incoming(float in);

	UFUNCTION(BlueprintCallable, Category = "Neural")
	void Outgoing(float out);

	UFUNCTION(BlueprintCallable, Category = "Neural")
	void OutgoingBase();

	DECLARE_EVENT_OneParam(UNeuralBackbone, FTriggerNeuron, float)
	FTriggerNeuron& OnTrigger() { return TriggerNeuron; };
	DECLARE_EVENT(UNeuralBackbone, FProcessData)
	FProcessData& OnProcess() { return ProcessData; };


	UFUNCTION(BlueprintCallable, Category = "Neural")
	void LinkProcessing(FProcessEvent data);



private:
	float currentCharge;
	bool fired;
	bool neg;

	
	FProcessData ProcessData;

	FTriggerNeuron TriggerNeuron;
};
