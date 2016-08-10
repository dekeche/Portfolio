// Fill out your copyright notice in the Description page of Project Settings.

#include "NeuralNetwork.h"
#include "EyeNode.h"
#include "Environment.h"

void AEyeNode::setup(ABaseOrganism* p, float minD, float maxD, float minA, float maxA, bool r, bool l, EOrganismType type)
{
	organism = p;
	right = r;
	left = l;

	maxDist = maxD;
	minDist = minD;
	minAngle = minA;
	maxAngle = maxA;

	testType = type;
};
void AEyeNode::DoProcess()
{
	AEnvironment* ref = AEnvironment::GetRef();

	TArray<ABaseOrganism*> list = ref->GetOrganisms(testType);
	int size = list.Num();
	float minDistCur = FLT_MAX;
	auto parent = organism;
	for (int i = 0; i < size; i++)
	{
		bool tLeft = false;
		bool tRight = false;
		ABaseOrganism* temp = list[i];
		if (temp != parent)
		{
			auto tVec = temp->GetActorLocation();
			auto pVec = parent->GetActorLocation();

			float absX = pVec.X - tVec.X;
			float absY = pVec.Y - tVec.Y;
			if (absX > (MaxWidth / 2))
				tVec.X +=MaxWidth;
			else if(absX < -(MaxWidth / 2))
				tVec.X -= MaxWidth;

			if (absY >(MaxHeight / 2))
				tVec.Y += MaxHeight;
			else if (absY < -(MaxHeight / 2))
				tVec.Y -= MaxHeight;
		
			float tempDist = FVector::Dist(tVec, pVec);
			if (tempDist < minDistCur && tempDist >= minDist && tempDist <= maxDist)
			{
				auto posVec = tVec - pVec;
				auto fwdVec = parent->GetActorForwardVector();
				auto rgtVec = parent->GetActorRightVector();

				posVec.Normalize();
				fwdVec.Normalize();
				rgtVec.Normalize();

				float angle = FVector::DotProduct(posVec,fwdVec);
				float rAngle = FVector::DotProduct(posVec, rgtVec);
			
				if (rAngle < 0.0f)
					tLeft = true;
				else
					tRight = true;

				if (((tLeft && left)||(tRight && right)) && angle >= minAngle && angle <= maxAngle)
				{
					minDistCur = tempDist;
				}
				//float tempAngle = temp->get
			}
		}
	}
	if (minDistCur <= maxDist)
	{
		TurnOn();
		minDistCur -= minDist;
		float out = 1.0f - ((minDistCur-minDist) / (maxDist - minDist));
		Neural->Outgoing(out);
	}
	else
	{
		TurnOff();
	}
};