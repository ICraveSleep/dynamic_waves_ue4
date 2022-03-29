// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicWaves/TriangleData.h"

FTriangleData::FTriangleData()
{
}

FTriangleData::~FTriangleData()
{
}

void FTriangleData::SetPointA(const FVector &Point)
{
	PointA = Point;	
}

void FTriangleData::SetPointB(const FVector &Point)
{
	PointB = Point;
}

void FTriangleData::SetPointC(const FVector &Point)
{
	PointC = Point;
}

FVector FTriangleData::GetPointA() const
{
	return PointA;
}

FVector FTriangleData::GetPointB() const
{
	return PointB;
}

FVector FTriangleData::GetPointC() const
{
	return PointC;
}

float FTriangleData::GetArea() const
{
	// float a = FVector::Distance(p2, p1);
	// float b = FVector::Distance(p3, p1);
	
	const float Distance_AB = FVector::Distance(PointB, PointA);
	const float Distance_AC = FVector::Distance(PointC, PointA);
	const float DotProduct = FVector::DotProduct(PointB-PointA, PointC-PointA) / (Distance_AB*Distance_AC);
	float VectorAngle = acos(DotProduct);
	if(VectorAngle < 0){
		UE_LOG(LogTemp, Warning, TEXT("Calculated angle that was negative: VectorAngle=%f"), VectorAngle);
		VectorAngle = 0;
	}
	float TriangleArea = 0.5*Distance_AB*Distance_AC*sin(VectorAngle); // is in cm^2
	TriangleArea = TriangleArea*0.00001; // Convert to m^2  //TODO(Sondre): Maybe all functions should return in cm^2 instead to follow UE standard
	return TriangleArea;
}

FVector FTriangleData::GetNormal() const
{
	// FVector normal = FVector::CrossProduct(p3-p1, p2-p1);
	FVector Normal = FVector::CrossProduct(PointC-PointA, PointB-PointA);
	Normal = Normal/Normal.Size(); // Normalising Vector
	return Normal;
}


FVector FTriangleData::GetCenter() const
{
	const FVector TriangleCenter = (PointA + PointB + PointC) / 3.0f;
	return TriangleCenter;
}
