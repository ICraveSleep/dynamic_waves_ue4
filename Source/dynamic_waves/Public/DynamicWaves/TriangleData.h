// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DYNAMIC_WAVES_API FTriangleData
{
public:
	FTriangleData();
	
	~FTriangleData();

	void SetPointA(const FVector &Point);
	void SetPointB(const FVector &Point);
	void SetPointC(const FVector &Point);
	
	FVector GetPointA() const;
	FVector GetPointB() const;
	FVector GetPointC() const;

	float GetArea() const;
	FVector GetNormal() const;
	FVector GetCenter() const;

private:
	FVector PointA;
	FVector PointB;
	FVector PointC;
};
