// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DYNAMIC_WAVES_API TriangleData
{
public:
	TriangleData(const FVector& A, const FVector& B, const FVector& C);
	~TriangleData();

private:
	FVector PointA;
	FVector PointB;
	FVector PointC;
};
