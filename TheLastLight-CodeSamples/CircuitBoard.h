// Project: Event Array Example
// Author: Jesse McKinley
// Date: 6-1-2016
// Target UNREAL Version 1.10.0

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "CircuitComponent.h"
#include "CircuitBreakerComponent.h"
#include "CircuitBoard.generated.h"
/**
 * 
 */


UDELEGATE(BlueprintType, Category = Delegate)
DECLARE_DYNAMIC_DELEGATE_OneParam(FStateEventResponder, bool, NewState);

UDELEGATE(BlueprintType, Category = Delegate)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStateChange, bool, NewState);

UDELEGATE(BlueprintType, Category = Delegate)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTrip);

USTRUCT()
struct FCircuitBreakerChannel
{
	GENERATED_USTRUCT_BODY()

	//what circuit does this breaker effect?
	UPROPERTY(EditAnywhere, Category = "BreakerChannel", meta = (DisplayName = "Circuit Target"))
	int32 m_CircuitTarget;

	//how much does this breaker draw? default = 1
	UPROPERTY(EditAnywhere, Category = "BreakerChannel", meta = (DisplayName = "Circuit Draw"))
	int32 m_CircuitDraw;
};


USTRUCT()
struct FCircuitBreakerBox
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "BreakerBox", meta = (DisplayName = "Breakers"))
	TArray<FCircuitBreakerChannel> m_Breakers;

	UPROPERTY()
	FTrip m_TripEvent;

	UPROPERTY(EditAnywhere, Category = "BreakerChannel", meta = (DisplayName = "Max Draw"))
	int32 m_MaxDraw;

	UPROPERTY()
	int32 m_CurrentDraw;

	UPROPERTY(EditAnywhere, Category = "BreakerChannel", meta = (DisplayName = "Breaker ID"))
	int32 m_BreakerNum = 0;
};

USTRUCT()
struct FCircuitEventChannel
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FStateChange m_StateEvent;

	UPROPERTY()
	bool m_CurrentState = false;
};

UCLASS()
class MYPROJECT_API ACircuitBoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACircuitBoard();
	~ACircuitBoard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitCircuit();

	//return functions
public:
	//Get the current circuit board, only one per level.
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	static ACircuitBoard* GetReference();

	//find a breaker index baised on ID.
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	int32 GetBreaker(int32 BreakerNum);

	//find the circuit index, baised on Breaker id, and the breaker channel #
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	int32 GetBreakerCircuit(int32 BreakerID, int32 BreakerChannel);

	//return the current state of a circuit ID.
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	bool GetCircuitState(int32 CircuitID);

	//register functions
public:
	//register component
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void RegisterCircuitComponent(int32 CircuitID, UCircuitComponent* target);

	//register breaker
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void RegisterBreaker(int32 BreakerID, int32 BreakerChannel, UCircuitBreakerComponent* target, bool OnOff);

	//trigger functions
public:

	//trigger circuit event
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void ChangeState(int32 CircuitID, bool State);

	//trigger breaker - circuit event
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void ChangeBreakerState(int32 BreakerID, int32 BreakerChannel, bool State);

	//trigger breaker trip event
	UFUNCTION(BlueprintCallable, Category = "Circuit")
	void TripBreaker(int32 BreakerID);

protected:
	//For use with time delay. (Initial lights)
	UPROPERTY()
	FTimerHandle m_TimerHandle;


	UPROPERTY()
	TArray<FCircuitEventChannel> m_EventChannels;

	//what circuits are initially on
	UPROPERTY(EditAnywhere, Category = "Circuit", meta = (DisplayName = "Initial Channels"))
	TArray<int32> m_InitialChannels;

	UPROPERTY(EditAnywhere, Category = "Circuit", meta = (DisplayName = "Breaker List"))
	TArray<FCircuitBreakerBox> m_BreakerList;

private:
	//refrence to on level circuit board.
	static ACircuitBoard* m_Singleton;
};
