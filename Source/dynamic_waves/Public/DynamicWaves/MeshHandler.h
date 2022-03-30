// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriangleData.h"
#include "VertexData.h"
#include "DrawDebugHelpers.h"


/**
 * 
 */
class DYNAMIC_WAVES_API FMeshHandler
{
public:
	FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn, UWorld* UWorld);
	// FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn, const ADynamicWaves* ActorComponentPtr);
	
	~FMeshHandler();
	
	void PrintMeshInfo();
	void DrawVertices();
	void UpdateMesh(const TArray<FVector>& MeshVertices, const FIndexArrayView& TriangleArrayIndex);
private:

	float GetWaveHeight(float x, float y);
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<int32_t> VerticesSurfaceDistance;
	int32_t NumberOfVertices;
	int32_t NumberOfTriangles;
	FIndexArrayView TrianglesIndexes;
	UWorld* WorldPointer;
	
};
