// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CircuitBreakerComponent.h"
#include "CircuitBoard.h"


// Sets default values for this component's properties
UCircuitBreakerComponent::UCircuitBreakerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCircuitBreakerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Setup();
}

void UCircuitBreakerComponent::Setup_Implementation()
{

	ACircuitBoard::GetReference()->RegisterBreaker(m_BreakerID, m_BreakerChannel, this, m_initialState);
	m_CurrentState = m_initialState;
}

void UCircuitBreakerComponent::ToggleBreaker()
{
	m_CurrentState = !m_CurrentState;
	ACircuitBoard::GetReference()->ChangeBreakerState(m_BreakerID, m_BreakerChannel, m_CurrentState);
};

void UCircuitBreakerComponent::TripBreaker()
{
	if (m_CurrentState)
	{
		m_CurrentState = false;
		DoTripBreaker();
	}
};

void UCircuitBreakerComponent::DoTripBreaker_Implementation()
{
	ACircuitBoard::GetReference()->ChangeBreakerState(m_BreakerID, m_BreakerChannel,false);
};

bool UCircuitBreakerComponent::GetState()
{
	return m_CurrentState;
};