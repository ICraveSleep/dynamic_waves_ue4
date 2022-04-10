// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicWaves/MeshHandler.h"

#include "PhysicsEngine/PhysicsSettings.h"


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

		// SurfaceDistance is checked against 0.0f, since 0.0f is at the surface of the wave, i.e the wave height
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

			//Add One triangle when One Vertex is under water
			if(VertexTriangle[0].SurfaceDistance > 0.0f && VertexTriangle[1].SurfaceDistance > 0.0f && VertexTriangle[2].SurfaceDistance < 0.0f)
			{
				AddOneSubTriangle(VertexTriangle);
			}
			//Add Two triangles when Two Vertices are under water
			else if(VertexTriangle[0].SurfaceDistance > 0.0f && VertexTriangle[1].SurfaceDistance < 0.0f && VertexTriangle[2].SurfaceDistance < 0.0f)
			{
				AddTwoSubTriangles(VertexTriangle);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("One to two vertices check fell through - check: void FMeshHandler::AddUnderWaterTriangles()"))	
			}
		}
	}
}

void FMeshHandler::DrawTriangles()
{
	for(uint16_t i = 0; i < UnderWaterTrianglesIndex; i++)
	{
		// Draw triangles
		DrawDebugLine(WorldPointer, UnderWaterTriangles[i].GetPointA(), UnderWaterTriangles[i].GetPointB(), FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, UnderWaterTriangles[i].GetPointB(), UnderWaterTriangles[i].GetPointC(), FColor{255,255,255}, false, 0.0f, 0, 2.0f);
		DrawDebugLine(WorldPointer, UnderWaterTriangles[i].GetPointC(), UnderWaterTriangles[i].GetPointA(), FColor{255,255,255}, false, 0.0f, 0, 2.0f);

		// Draw triangle center dots
		DrawDebugPoint(WorldPointer, UnderWaterTriangles[i].GetCenter(), 15.0f, FColor(0, 0, 0),false, 0.0f);

		//Draw normal vector
		DrawDebugLine(WorldPointer, UnderWaterTriangles[i].GetCenter(), UnderWaterTriangles[i].GetCenter() + UnderWaterTriangles[i].GetNormal()*100.0f, FColor::Green, false, 0.0f, 0, 2.0f);

		//Draw Red dots
		DrawDebugPoint(WorldPointer, UnderWaterTriangles[i].GetPointA(), 15.0f, FColor::Red,false, 0.0f);
		DrawDebugPoint(WorldPointer, UnderWaterTriangles[i].GetPointB(), 15.0f, FColor::Red,false, 0.0f);
		DrawDebugPoint(WorldPointer, UnderWaterTriangles[i].GetPointC(), 15.0f, FColor::Red,false, 0.0f);
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

void FMeshHandler::AddOneSubTriangle(const TArray<FVertexData>& VerticesData)
{
	//The received VerticesData reference has sorted them by descending value (0 > 1 > 2). 
	FVector Upper;
	FVector Middle;
	FVector Lower = VerticesData[2].WorldVertexLocation;
	uint8_t UpperIndex = VerticesData[2].Index + 1;

	if(UpperIndex > 2)
	{
		UpperIndex = 0;
	}
	
	float SurfaceDistanceUpper;
	float SurfaceDistanceMiddle;
	float SurfaceDistanceLower = VerticesData[2].SurfaceDistance;

	if(VerticesData[1].Index == UpperIndex)
	{
		Upper = VerticesData[0].WorldVertexLocation;
		Middle = VerticesData[1].WorldVertexLocation;

		SurfaceDistanceUpper = VerticesData[0].SurfaceDistance;
		SurfaceDistanceMiddle = VerticesData[1].SurfaceDistance;
	}
	else
	{
		Upper = VerticesData[1].WorldVertexLocation;
		Middle = VerticesData[0].WorldVertexLocation;

		SurfaceDistanceUpper = VerticesData[1].SurfaceDistance;
		SurfaceDistanceMiddle = VerticesData[0].SurfaceDistance;
	}

	// Cutting Algorithm from: https://www.gamedeveloper.com/programming/water-interaction-model-for-boats-in-video-games
	//Point J_M
	FVector LM = Middle - Lower;
	float t_M = -SurfaceDistanceLower / (SurfaceDistanceMiddle - SurfaceDistanceLower);
	FVector LJ_M = t_M * LM;
	FVector J_M = LJ_M + Lower;
	//Point J_H
	FVector LH = Upper - Lower;
	float t_H = -SurfaceDistanceLower / (SurfaceDistanceUpper - SurfaceDistanceLower);
	FVector LJ_H = t_H * LH;
	FVector J_H = LJ_H + Lower;
	
	UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointA(J_H);
	UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointB(Lower);
	UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointC(J_M);
	++UnderWaterTrianglesIndex;
}

void FMeshHandler::AddTwoSubTriangles(const TArray<FVertexData>& VerticesData)
{
	
	//The received VerticesData reference has sorted them by descending value (0 > 1 > 2). 
	FVector Upper =  VerticesData[0].WorldVertexLocation;
	FVector Middle;
	FVector Lower;
	
	int8_t MiddleIndex = VerticesData[0].Index - 1;
	if(MiddleIndex < 0)
	{
		MiddleIndex = 2;
	}
	
	float SurfaceDistanceUpper = VerticesData[0].SurfaceDistance;
	float SurfaceDistanceMiddle;
	float SurfaceDistanceLower;

	if(VerticesData[1].Index == MiddleIndex)
	{
		Middle = VerticesData[2].WorldVertexLocation;
		Lower = VerticesData[1].WorldVertexLocation;

		SurfaceDistanceMiddle = VerticesData[2].SurfaceDistance;
		SurfaceDistanceLower = VerticesData[1].SurfaceDistance;
	}
	else
	{
		Middle = VerticesData[1].WorldVertexLocation;
		Lower = VerticesData[2].WorldVertexLocation;

		SurfaceDistanceMiddle = VerticesData[1].SurfaceDistance;
		SurfaceDistanceLower = VerticesData[2].SurfaceDistance;
	}

	// Cutting Algorithm from: https://www.gamedeveloper.com/programming/water-interaction-model-for-boats-in-video-games
	//Point I_M
	FVector MH = Upper - Middle;
	float t_M = -SurfaceDistanceMiddle / (SurfaceDistanceUpper - SurfaceDistanceMiddle);
	FVector MI_M = t_M * MH;
	FVector I_M = MI_M + Middle;
	//Point I_L
	FVector LH = Upper - Lower;
	float t_L = -SurfaceDistanceLower / (SurfaceDistanceUpper - SurfaceDistanceLower);
	FVector LI_L = t_L * LH;
	FVector I_L = LI_L + Lower;
	

	UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointA(Middle);
    UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointB(Lower);
    UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointC(I_L);
	++UnderWaterTrianglesIndex;
	
	
	UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointA(Middle);
    UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointB(I_L);
    UnderWaterTriangles[UnderWaterTrianglesIndex].SetPointC(I_M);
	++UnderWaterTrianglesIndex;
}



float FMeshHandler::GetWaveHeight(float x, float y)
{
	float WorldTime = WorldPointer->GetTimeSeconds();
	FVector2D WaveDirection = {1.0f, 0.0f};
	WaveDirection.Normalize();
	FVector2D BoatPosXY = {x, y};
	float PlaneLocation = FVector2D::DotProduct(BoatPosXY, WaveDirection);
	float WaveHeight = WaveAmplitude*sin(0.01*(PI*2/48)*PlaneLocation + WorldTime);
	return WaveHeight;
	// return 0.0f; // TODO(Sondre): Currently using flat water
}

void FMeshHandler::AddForces(UStaticMeshComponent* Mesh)
{
	
	for(int i = 0; i < UnderWaterTrianglesIndex - 1; i++){
		
		FVector Force =  FluidDensity * Gravity * (UnderWaterTriangles[i].GetCenter().Z - GetWaveHeight(UnderWaterTriangles[i].GetCenter().X, UnderWaterTriangles[i].GetCenter().Y)) * UnderWaterTriangles[i].GetArea() * UnderWaterTriangles[i].GetNormal();
		
		Force.X = 0.0f;
		Force.Y = 0.0f;
		// UPhysicsSettings::Get()->DefaultGravityZ;
		Mesh->AddForceAtLocation(Force, UnderWaterTriangles[i].GetCenter(), NAME_None);
		//UE_LOG(LogTemp, Warning, TEXT("Force: %f"), Force.Z);
		FVector drag = {0.0f, 0.0f, 0.0f};
		float C = 0.000000128f;
		float R = 0;
		FVector V = Mesh->GetComponentVelocity();
		
		FVector W = Mesh->GetPhysicsAngularVelocityInRadians();
		FVector R_BA = UnderWaterTriangles[i].GetCenter() - Mesh->GetCenterOfMass();
		FVector V_A =  V + FVector::CrossProduct(W, R_BA);
		
		if(V_A.Z < 0)
		{
			R = 0.5f*UnderWaterTriangles[i].GetArea()*1027.0f*C*V_A.Z*V_A.Z;	
		}
		else
		{
			R = -0.5f*UnderWaterTriangles[i].GetArea()*1027.0f*C*V_A.Z*V_A.Z;
		}
			
		drag.Z = R;
		Mesh->AddForceAtLocation(drag, UnderWaterTriangles[i].GetCenter(), NAME_None);
	}
}


