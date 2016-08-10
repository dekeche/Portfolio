// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CircuitBoard.h"
#include "CircuitComponent.h"


// Sets default values for this component's properties
UCircuitComponent::UCircuitComponent()
{
	bWantsBeginPlay = true;

	//this component just interacts with the circuit board.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCircuitComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	Setup();
}

void UCircuitComponent::Setup_Implementation()
{

	ACircuitBoard* BoardRef = ACircuitBoard::GetReference();
	BoardRef->RegisterCircuitComponent(Circuit, this);
};

void UCircuitComponent::ChangeState(bool newState)
{
	DoChangeState(newState);
};


void UCircuitComponent::DoChangeState_Implementation(bool newState)
{
	//possible implementations;
	//callback to base class
	//event broadcast, register parent class with event
};