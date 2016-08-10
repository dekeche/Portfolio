// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CircuitComponent.generated.h"


UCLASS()
class MYPROJECT_API UCircuitComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCircuitComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LightSetup")
	void Setup();
	virtual void Setup_Implementation();


	UFUNCTION()
	void ChangeState(bool newState);


	/*
	* This function is used to implement the functionallity 
	* for when the change state event is triggered.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LightEvent")
	void DoChangeState(bool newState);
	virtual void DoChangeState_Implementation(bool newState);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CircuitEvents")
	int32 Circuit;
};
