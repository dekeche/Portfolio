// Fill out your copyright notice in the Description page of Project Settings.

#include "NeuralNetwork.h"
#include "OutNode.h"




void AOutNode::SetVector(FVector dir) 
{
	mDir = dir;
};
FVector AOutNode::GetMovement() 
{
	if (active)
	{
		return mDir;
	}
	return FVector(0, 0, 0);
};

void AOutNode::DoReset() 
{
	active = false;
};

void AOutNode::DoProcess() 
{
	active = true;
};