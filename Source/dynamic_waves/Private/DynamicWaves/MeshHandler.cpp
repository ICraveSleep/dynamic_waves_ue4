// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicWaves/MeshHandler.h"


// FMeshHandler::FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn, const ADynamicWaves* ActorComponentPtr) : ActorPointer(ActorComponentPtr)
FMeshHandler::FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn, UWorld* UWorld) : WorldPointer(UWorld)
{
	TriangleSize = TriangleSizeIn;
	VertexSize = VertexSizeIn;
	const FVector Initialization = {0.0f,0.0f,0.0f};
	Vertices.SetNum(VertexSize);
	UE_LOG(LogTemp, Warning, TEXT("Init size: %i"), Vertices.Num());
	
}

FMeshHandler::~FMeshHandler()
{
}

void FMeshHandler::PrintMeshInfo()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
	                                 FString::Printf(
		                                 TEXT("VertexSize: %i, TriangleSize: %i"), VertexSize, TriangleSize));
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}

void FMeshHandler::DrawVertices()
{
	for(int i = 1; i < TrianglesIndexes.Num(); i=i+3){
		float x_sum = Vertices[TrianglesIndexes[i-1]].X + Vertices[TrianglesIndexes[i]].X + Vertices[TrianglesIndexes[i+1]].X;
		float y_sum = Vertices[TrianglesIndexes[i-1]].Y + Vertices[TrianglesIndexes[i]].Y + Vertices[TrianglesIndexes[i+1]].Y;
		float z_sum = Vertices[TrianglesIndexes[i-1]].Z + Vertices[TrianglesIndexes[i]].Z + Vertices[TrianglesIndexes[i+1]].Z;
		FVector center = {x_sum/3.0f, y_sum/3.0f, z_sum/3.0f};
		
		FVector p1 = Vertices[TrianglesIndexes[i-1]];
		FVector p2 = Vertices[TrianglesIndexes[i]];
		FVector p3 = Vertices[TrianglesIndexes[i+1]];
		FVector normal = FVector::CrossProduct(p3-p1, p2-p1);
		// if(DrawNormals)
		// {
		// DrawDebugLine(WorldPointer, center, center + normal/normal.Size() * 50.0f, FColor{0,0,255}, false, 0.0f, 0, 2.0f);
		// }
		DrawDebugPoint(WorldPointer, center, 15.0f, FColor(0, 0, 0),false, 0.0f);
		DrawDebugLine(WorldPointer, Vertices[TrianglesIndexes[i-1]], Vertices[TrianglesIndexes[i]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, Vertices[TrianglesIndexes[i]], Vertices[TrianglesIndexes[i+1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, Vertices[TrianglesIndexes[i+1]], Vertices[TrianglesIndexes[i-1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
	}
}
		

void FMeshHandler::UpdateMesh(const TArray<FVector>& MeshVertices, const FIndexArrayView& TriangleArrayIndex)
{
	Vertices = MeshVertices;
	TrianglesIndexes = TriangleArrayIndex;
	int32 test = TrianglesIndexes[0];
	for(int32_t i = 0; i < MeshVertices.Num(); i++){
		DrawDebugPoint(WorldPointer->GetWorld(), Vertices[i], 15.0f, FColor(255, 0, 0),false, 0.0f);
	}
	
	for(int i = 1; i < TrianglesIndexes.Num(); i=i+3){
        float x_sum = Vertices[TrianglesIndexes[i-1]].X + Vertices[TrianglesIndexes[i]].X + Vertices[TrianglesIndexes[i+1]].X;
        float y_sum = Vertices[TrianglesIndexes[i-1]].Y + Vertices[TrianglesIndexes[i]].Y + Vertices[TrianglesIndexes[i+1]].Y;
        float z_sum = Vertices[TrianglesIndexes[i-1]].Z + Vertices[TrianglesIndexes[i]].Z + Vertices[TrianglesIndexes[i+1]].Z;
        FVector center = {x_sum/3.0f, y_sum/3.0f, z_sum/3.0f};
		
        FVector p1 = Vertices[TrianglesIndexes[i-1]];
        FVector p2 = Vertices[TrianglesIndexes[i]];
        FVector p3 = Vertices[TrianglesIndexes[i+1]];
        FVector normal = FVector::CrossProduct(p3-p1, p2-p1);
        // if(DrawNormals)
        // {
        // DrawDebugLine(WorldPointer, center, center + normal/normal.Size() * 50.0f, FColor{0,0,255}, false, 0.0f, 0, 2.0f);
        // }
        DrawDebugPoint(WorldPointer, center, 15.0f, FColor(0, 0, 0),false, 0.0f);
        DrawDebugLine(WorldPointer, Vertices[TrianglesIndexes[i-1]], Vertices[TrianglesIndexes[i]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
        DrawDebugLine(WorldPointer, Vertices[TrianglesIndexes[i]], Vertices[TrianglesIndexes[i+1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
        DrawDebugLine(WorldPointer, Vertices[TrianglesIndexes[i+1]], Vertices[TrianglesIndexes[i-1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
    }

	UE_LOG(LogTemp, Warning, TEXT("USize: %i LSize: %i"), MeshVertices.Num(), Vertices.Num());
}

float FMeshHandler::GetWaveHeight(float x, float y)
{
	float WorldTime = WorldPointer->GetTimeSeconds();
	FVector2D WaveDirection = {1.0f, 0.0f};
	WaveDirection.Normalize();
	FVector2D BoatPosXY = {x, y};
	float PlaneLocation = FVector2D::DotProduct(BoatPosXY, WaveDirection);
	float WaveHeight = 150.0f*sin(0.01*(PI*2/48)*PlaneLocation + WorldTime);
	return WaveHeight;
}