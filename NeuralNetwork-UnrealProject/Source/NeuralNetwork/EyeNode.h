// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseNode.h"
#include "HerbOrganism.h"
#include "EyeNode.generated.h"

/**
 * 
 */
UCLASS()
class NEURALNETWORK_API AEyeNode : public ABaseNode
{
	GENERATED_BODY()
	
public:

	void setup(ABaseOrganism* p, float minD,float maxD,float minA,float maxA, bool r, bool l ,EOrganismType type);
	void DoProcess();
	
private:
	ABaseOrganism* organism;

	bool right;
	bool left;

	float maxDist;
	float minDist;
	float minAngle;
	float maxAngle;

	EOrganismType testType;
};
