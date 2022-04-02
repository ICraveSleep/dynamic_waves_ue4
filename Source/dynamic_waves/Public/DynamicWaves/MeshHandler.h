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
	FMeshHandler(FIndexArrayView IndexArrayView, uint32_t VertexSizeIn, uint32_t TriangleSizeIn, UWorld* UWorld);
	// FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn, const ADynamicWaves* ActorComponentPtr);
	
	~FMeshHandler();
	
	void PrintMeshInfo();
	void DrawVertices();
	void UpdateMeshTriangles(const TArray<FVector>& MeshVertices);
private:

	void AddUnderWaterTriangles();
	void DrawTriangles();
	
	void AddOneSubTriangle(const TArray<FVertexData>& VerticesData);
	void AddTwoSubTriangles(const TArray<FVertexData>& VerticesData);
	TArray<FVertexData> SortVerticesByDistance(const TArray<FVertexData>& VerticesData);
	float GetWaveHeight(float x, float y);
	TArray<FVector> Vertices;
	TArray<FTriangleData> UnderWaterTriangles;
	uint16_t UnderWaterTrianglesIndex;
	TArray<int32> Triangles;
	TArray<float> VerticesSurfaceDistance;
	int32_t NumberOfVertices;
	int32_t NumberOfTriangles;
	FIndexArrayView TriangleIndexes;
	UWorld* WorldPointer;
	
};
