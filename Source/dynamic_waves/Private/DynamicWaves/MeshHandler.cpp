// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicWaves/MeshHandler.h"


// FMeshHandler::FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn, const ADynamicWaves* ActorComponentPtr) : ActorPointer(ActorComponentPtr)
FMeshHandler::FMeshHandler(FIndexArrayView IndexArrayView, uint32_t VertexSizeIn, uint32_t TriangleSizeIn, UWorld* UWorld) : WorldPointer(UWorld)
{
	TriangleIndexes = IndexArrayView;
	NumberOfTriangles = TriangleSizeIn;
	NumberOfVertices = VertexSizeIn;
	UnderWaterTrianglesIndex = 0;
	Vertices.SetNum(NumberOfVertices);
	UnderWaterTriangles.SetNum(NumberOfVertices*3);
	VerticesSurfaceDistance.SetNum(NumberOfVertices);
	UE_LOG(LogTemp, Warning, TEXT("Init size: %i"), Vertices.Num());
}

FMeshHandler::~FMeshHandler()
{
}

void FMeshHandler::PrintMeshInfo()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
	                                 FString::Printf(
		                                 TEXT("VertexSize: %i, TriangleSize: %i"), NumberOfVertices, NumberOfTriangles));
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}

void FMeshHandler::DrawVertices()
{
	for(int i = 1; i < TriangleIndexes.Num(); i=i+3){
		float x_sum = Vertices[TriangleIndexes[i-1]].X + Vertices[TriangleIndexes[i]].X + Vertices[TriangleIndexes[i+1]].X;
		float y_sum = Vertices[TriangleIndexes[i-1]].Y + Vertices[TriangleIndexes[i]].Y + Vertices[TriangleIndexes[i+1]].Y;
		float z_sum = Vertices[TriangleIndexes[i-1]].Z + Vertices[TriangleIndexes[i]].Z + Vertices[TriangleIndexes[i+1]].Z;
		FVector center = {x_sum/3.0f, y_sum/3.0f, z_sum/3.0f};
		
		FVector p1 = Vertices[TriangleIndexes[i-1]];
		FVector p2 = Vertices[TriangleIndexes[i]];
		FVector p3 = Vertices[TriangleIndexes[i+1]];
		FVector normal = FVector::CrossProduct(p3-p1, p2-p1);
		// if(DrawNormals)
		// {
		// DrawDebugLine(WorldPointer, center, center + normal/normal.Size() * 50.0f, FColor{0,0,255}, false, 0.0f, 0, 2.0f);
		// }
		DrawDebugPoint(WorldPointer, center, 15.0f, FColor(0, 0, 0),false, 0.0f);
		DrawDebugLine(WorldPointer, Vertices[TriangleIndexes[i-1]], Vertices[TriangleIndexes[i]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, Vertices[TriangleIndexes[i]], Vertices[TriangleIndexes[i+1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, Vertices[TriangleIndexes[i+1]], Vertices[TriangleIndexes[i-1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
	}
}
		

void FMeshHandler::UpdateMeshTriangles(const TArray<FVector>& MeshVertices)
{
	Vertices = MeshVertices;
	// TriangleIndexes = TriangleArrayIndex;
	float IterationTime = WorldPointer->GetWorld()->TimeSeconds;
	
	float WaveHeightAtLocation;
	for(int32_t i = 0; i < NumberOfVertices; i++){
		WaveHeightAtLocation = GetWaveHeight(Vertices[i].X, Vertices[i].Y);
		WaveHeightAtLocation = Vertices[i].Z - WaveHeightAtLocation;  // The sign of this height indicates if the vertex is above(+) or below(-) water
		VerticesSurfaceDistance[i] = WaveHeightAtLocation;
	}

	AddUnderWaterTriangles();
	DrawTriangles();
}

void FMeshHandler::AddUnderWaterTriangles()
{
	UnderWaterTrianglesIndex = 0;
	
	TArray<FVertexData> VertexTriangle = {
		{0.0f, 0, FVector(0.0f, 0.0f, 0.0f)},
		{0.0f, 0, FVector(0.0f, 0.0f, 0.0f)},
		{0.0f, 0, FVector(0.0f, 0.0f, 0.0f)}};

	for(int i = 0; i < TriangleIndexes.Num(); i=i+3)
	{
		VertexTriangle[0].Index = 0;
		VertexTriangle[0].SurfaceDistance = VerticesSurfaceDistance[TriangleIndexes[i]];
		VertexTriangle[0].WorldVertexLocation = Vertices[TriangleIndexes[i]];

		VertexTriangle[1].Index = 1;
		VertexTriangle[1].SurfaceDistance = VerticesSurfaceDistance[TriangleIndexes[i+1]];
		VertexTriangle[1].WorldVertexLocation = Vertices[TriangleIndexes[i+1]];

		VertexTriangle[2].Index = 2;
		VertexTriangle[2].SurfaceDistance = VerticesSurfaceDistance[TriangleIndexes[i+2]];
		VertexTriangle[2].WorldVertexLocation = Vertices[TriangleIndexes[i+2]];
		//UE_LOG(LogTemp, Warning, TEXT("Vertices: {%f, %f, %f}"), VertexTriangle[0].SurfaceDistance, VertexTriangle[1].SurfaceDistance, VertexTriangle[2].SurfaceDistance)
		//UE_LOG(LogTemp, Warning, TEXT("Vertices: {%f, %f, %f}"), VerticesSurfaceDistance[TriangleIndexes[i]], VerticesSurfaceDistance[TriangleIndexes[i+1]], VerticesSurfaceDistance[TriangleIndexes[i+2]]);

		//Continue to next triangle if all vertices are above water
		if(VertexTriangle[0].SurfaceDistance >= 0.0f && VertexTriangle[1].SurfaceDistance >= 0.0f && VertexTriangle[2].SurfaceDistance >= 0.0f)
		{
			continue;
		}

		//If all vertices are below water store it directly
		if(VertexTriangle[0].SurfaceDistance < 0.0f && VertexTriangle[1].SurfaceDistance < 0.0f && VertexTriangle[2].SurfaceDistance < 0.0f)
		{
			UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointA(VertexTriangle[0].WorldVertexLocation);
			UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointB(VertexTriangle[1].WorldVertexLocation);
			UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointC(VertexTriangle[2].WorldVertexLocation);
			++UnderWaterTrianglesIndex;
		}
		else
		{
			// Sort the vertexes surface distances to easier check how many are under water
			VertexTriangle = SortVerticesByDistance(VertexTriangle);
			
			
		}
	}
}

void FMeshHandler::DrawTriangles()
{
	for (uint32_t i = 0; i < UnderWaterTrianglesIndex; i++)
	{
		DrawDebugLine(WorldPointer, UnderWaterTriangles[i].GetPointA(), UnderWaterTriangles[i].GetPointB(), FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, UnderWaterTriangles[i].GetPointB(), UnderWaterTriangles[i].GetPointC(), FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, UnderWaterTriangles[i].GetPointC(), UnderWaterTriangles[i].GetPointA(), FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugPoint(WorldPointer, UnderWaterTriangles[i].GetCenter(), 15.0f, FColor(0, 0, 0),false, 0.0f);
	}	
}

TArray<FVertexData> FMeshHandler::SortVerticesByDistance(const TArray<FVertexData>& VerticesData)
{
	if(VerticesData[0].SurfaceDistance >= VerticesData[1].SurfaceDistance && VerticesData[0].SurfaceDistance >= VerticesData[2].SurfaceDistance)
	{
		if(VerticesData[1].SurfaceDistance >= VerticesData[2].SurfaceDistance)
		{
			return {VerticesData[0], VerticesData[1], VerticesData[2]};
		}
		else
		{
			return {VerticesData[0], VerticesData[2], VerticesData[1]};
		}
	}
	else if (VerticesData[1].SurfaceDistance >= VerticesData[0].SurfaceDistance && VerticesData[1].SurfaceDistance >= VerticesData[2].SurfaceDistance)
	{
		if(VerticesData[0].SurfaceDistance >= VerticesData[2].SurfaceDistance)
		{
			return {VerticesData[1], VerticesData[0], VerticesData[2]};
		}
		else
		{
			return {VerticesData[1], VerticesData[2], VerticesData[0]};
		}
	}
	else
	{
		if(VerticesData[0].SurfaceDistance >= VerticesData[1].SurfaceDistance)
		{
			return {VerticesData[2], VerticesData[0], VerticesData[1]};
		}
		else
		{
			return {VerticesData[2], VerticesData[1], VerticesData[0]};
		}		
	}
}

float FMeshHandler::GetWaveHeight(float x, float y)
{
	float WorldTime = WorldPointer->GetTimeSeconds();
	FVector2D WaveDirection = {1.0f, 0.0f};
	WaveDirection.Normalize();
	FVector2D BoatPosXY = {x, y};
	float PlaneLocation = FVector2D::DotProduct(BoatPosXY, WaveDirection);
	float WaveHeight = 150.0f*sin(0.01*(PI*2/48)*PlaneLocation + WorldTime);
	// return WaveHeight;

	return 0.0f; // TODO(Sondre): Currently using flat water
}



