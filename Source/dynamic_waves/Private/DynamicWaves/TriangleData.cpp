// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicWaves/TriangleData.h"

TriangleData::TriangleData(const FVector& A, const FVector& B, const FVector& C)
{
	PointA = A;
	PointB = B;
	PointC = C;
}

TriangleData::~TriangleData()
{
}
