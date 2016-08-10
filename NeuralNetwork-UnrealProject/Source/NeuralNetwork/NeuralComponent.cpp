// Fill out your copyright notice in the Description page of Project Settings.

#include "NeuralNetwork.h"
#include "NeuralComponent.h"


// Sets default values for this component's properties
UNeuralComponent::UNeuralComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UNeuralComponent::SetFireValues(float incoming, float calmDown, float outgoing, bool negative)
{
	currentCharge = BaseCharge = 0.0f;
	IncomingFireTrigger = incoming;
	CalmDownRate = calmDown;
	OutgoingFire = outgoing;
	neg = negative;
};
void UNeuralComponent::BeginPlay()
{
	if (currentCharge != BaseCharge)
	{
		currentCharge -= CalmDownRate;
		if (currentCharge < BaseCharge)
		{
			currentCharge = BaseCharge;
		}
	}
	fired = false;
}

void UNeuralComponent::Process()
{
	if(currentCharge > 0.0f)
	{
		currentCharge -= CalmDownRate;
		if (currentCharge < 0.0f)
			currentCharge = 0.0f;
	}
	if (currentCharge > 1.0f)
	{
		currentCharge = 1.0f;
	}
	fired = false;
};

void UNeuralComponent::LinkIncoming(UNeuralComponent* ref)
{
	TriggerNeuron.AddUObject(ref, &UNeuralComponent::Incoming);
};

void UNeuralComponent::Incoming(float in)
{
	if (!fired)
	{
		fired = true;
		currentCharge += in;

		if (!neg)
		{
			if (currentCharge >= IncomingFireTrigger)
			{
				ProcessData.Broadcast();
			}
		}
		else
		{
			if (currentCharge < IncomingFireTrigger)
			{
				ProcessData.Broadcast();
			}
		}
	}
};
void UNeuralComponent::Outgoing(float out)
{
	TriggerNeuron.Broadcast(out);
};

void UNeuralComponent::OutgoingBase()
{
	TriggerNeuron.Broadcast(OutgoingFire);
};
void UNeuralComponent::LinkProcessing(FProcessEvent data)
{
	ProcessData.AddUFunction(data.GetUObject(), data.GetFunctionName());
};