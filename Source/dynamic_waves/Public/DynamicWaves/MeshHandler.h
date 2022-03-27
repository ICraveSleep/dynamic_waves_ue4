// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DYNAMIC_WAVES_API FMeshHandler
{
public:
	FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn);
	
	~FMeshHandler();
	
	void PrintMeshInfo();
private:
	
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	uint32_t VertexSize;
	uint32_t TriangleSize;
};
