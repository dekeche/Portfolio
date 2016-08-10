// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CircuitBreakerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UCircuitBreakerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCircuitBreakerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CircuitBreaker")
	void Setup();
	virtual void Setup_Implementation();

	UFUNCTION()
	void ToggleBreaker();

	UFUNCTION()
	void TripBreaker();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CircuitBreaker")
	void DoTripBreaker();
	virtual void DoTripBreaker_Implementation();

	UFUNCTION()
	bool GetState();

protected:

	//what circuits are initially on
	UPROPERTY(EditAnywhere, Category = "CircuitBreaker", meta = (DisplayName = "Initial State"))
	bool m_initialState;

	UPROPERTY(EditAnywhere, Category = "CircuitBreaker", meta = (DisplayName = "Breaker ID"))
	bool m_BreakerID;

	UPROPERTY(EditAnywhere, Category = "CircuitBreaker", meta = (DisplayName = "Breaker Channel"))
	bool m_BreakerChannel;

	bool m_CurrentState;

};
