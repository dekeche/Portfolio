// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CircuitBoard.h"
ACircuitBoard* ACircuitBoard::m_Singleton = nullptr;
ACircuitBoard::ACircuitBoard()
{
	PrimaryActorTick.bCanEverTick = false;

	//Singleton can only by non-null when loading a second level.
	m_Singleton = this;
}

ACircuitBoard::~ACircuitBoard()
{
	//logic check
	if(m_Singleton == this)
		m_Singleton = nullptr;
}

void ACircuitBoard::BeginPlay()
{
	Super::BeginPlay();

	//sets a delay, so that all objects on the level can register themselves first,
	//before handeling the initial conxition of the circuit system
	GetWorldTimerManager().SetTimer(m_TimerHandle, m_Singleton, &ACircuitBoard::InitCircuit, 0.05f, false);
}

void ACircuitBoard::InitCircuit()
{
	//preset all circuit channels to off
	for (FCircuitEventChannel channel : m_EventChannels)
	{
		channel.m_StateEvent.Broadcast(false);
		channel.m_CurrentState = false;
	}

	//turn on iniital channels
	for (int active : m_InitialChannels)
	{
		m_EventChannels[active].m_CurrentState = true;
		m_EventChannels[active].m_StateEvent.Broadcast(true);
	}
};
#pragma region ReturnFuncs
ACircuitBoard* ACircuitBoard::GetReference()
{
	return m_Singleton;
};

int32 ACircuitBoard::GetBreaker(int32 BreakerNum)
{
	int32 size = m_BreakerList.Num();

	//search for breaker id
	for (int i = 0; i < size; i++)
	{
		if (m_BreakerList[i].m_BreakerNum == BreakerNum)
			return i;
	}

	//breaker id not found
	return 0;
};

int32 ACircuitBoard::GetBreakerCircuit(int32 BreakerID, int32 BreakerChannel)
{

	int32 size = m_BreakerList.Num();

	if (BreakerID >= size)
		return -1;

	size = m_BreakerList[BreakerID].m_Breakers.Num();


	if (BreakerChannel >= size)
		return -1;

	return m_BreakerList[BreakerID].m_Breakers[BreakerChannel].m_CircuitTarget;
};

bool ACircuitBoard::GetCircuitState(int32 CircuitID)
{
	int32 size = m_EventChannels.Num();


	if (CircuitID >= size)
		return false;

	return m_EventChannels[CircuitID].m_CurrentState;
};
#pragma endregion ReturnFuncs


#pragma region RegisterFuncs

void ACircuitBoard::RegisterCircuitComponent(int32 CircuitID, UCircuitComponent* target)
{
	int32 size = m_EventChannels.Num();

	if (size <= CircuitID)
	{
		m_EventChannels.SetNumZeroed(CircuitID + 1);
	}

	m_EventChannels[CircuitID].m_StateEvent.AddDynamic(target, &UCircuitComponent::ChangeState);

};

void ACircuitBoard::RegisterBreaker(int32 BreakerID, int32 BreakerChannel, UCircuitBreakerComponent* target, bool OnOff)
{
	if (GetBreakerCircuit(BreakerID, BreakerChannel) < 0)
		return;
	m_BreakerList[BreakerID].m_TripEvent.AddDynamic(target, &UCircuitBreakerComponent::TripBreaker);
};
#pragma endregion RegisterFuncs


#pragma region EventFuncs

//trigger circuit event
void ACircuitBoard::ChangeState(int32 CircuitID, bool State)
{
	int32 size = m_EventChannels.Num();

	if (size <= CircuitID)
	{
		return;
	}

	m_EventChannels[CircuitID].m_StateEvent.Broadcast(State);
};

//trigger breaker - circuit event
void ACircuitBoard::ChangeBreakerState(int32 BreakerID, int32 BreakerChannel, bool State)
{
	int32 CircuitID = GetBreakerCircuit(BreakerID, BreakerChannel);
	int32 size = m_EventChannels.Num();

	if (size <= CircuitID || CircuitID < 0)
	{
		return;
	}

	m_EventChannels[CircuitID].m_StateEvent.Broadcast(State);

	if (State)
	{
		m_BreakerList[BreakerID].m_CurrentDraw += m_BreakerList[BreakerID].m_Breakers[BreakerChannel].m_CircuitDraw;
		if (m_BreakerList[BreakerID].m_CurrentDraw > m_BreakerList[BreakerID].m_MaxDraw)
		{
			TripBreaker(BreakerID);
		}
	}
	else
	{
		m_BreakerList[BreakerID].m_CurrentDraw -= m_BreakerList[BreakerID].m_Breakers[BreakerChannel].m_CircuitDraw;
	}
};

//trigger breaker trip event
void ACircuitBoard::TripBreaker(int32 BreakerID)
{
	int32 size = m_BreakerList.Num();

	if (BreakerID >= size)
		return;

	m_BreakerList[BreakerID].m_TripEvent.Broadcast();
};

#pragma endregion EventFuncs