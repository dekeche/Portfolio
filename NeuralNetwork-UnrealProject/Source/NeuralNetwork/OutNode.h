// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseNode.h"
#include "OutNode.generated.h"

/**
 * 
 */
UCLASS()
class NEURALNETWORK_API AOutNode : public ABaseNode
{
	GENERATED_BODY()
public:

	void SetVector(FVector dir);
	FVector GetMovement();

	void DoReset();
	void DoProcess();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Neural")
	FVector mDir;

	bool active;
};
