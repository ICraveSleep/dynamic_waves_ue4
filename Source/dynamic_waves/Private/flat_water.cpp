// Fill out your copyright notice in the Description page of Project Settings.


#include "flat_water.h"
#include "../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/clsocket/src/ActiveSocket.h"

// Sets default values
Aflat_water::Aflat_water()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root_secne_component = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = root_secne_component;
	
	proc_mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	proc_mesh->SetupAttachment(RootComponent);
	
	
}

// Called when the game starts or when spawned
void Aflat_water::BeginPlay(){
	Super::BeginPlay();
	GenerateMesh();
}


// This is called when actor is spawned (at runtime or when you drop it into the world in editor)
void Aflat_water::PostActorCreated(){
	Super::PostActorCreated();
	GenerateMesh();
}

// This is called when actor is already in level and map is opened
void Aflat_water::PostLoad(){
	Super::PostLoad();
	GenerateMesh();
}


// Called every frame
void Aflat_water::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	
}

void Aflat_water::GenerateMesh(){
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();
	UVs.Reset();
	Colors.Reset();

	int32 TriangleIndexCount = 0;
	FVector DefinedShape[3];
	FProcMeshTangent TangentSetup;

	DefinedShape[0] = FVector(TrianglePositions.X, TrianglePositions.Y, TrianglePositions.Z);
	DefinedShape[1] = FVector(TrianglePositions.X, TrianglePositions.Y, -TrianglePositions.Z);
	DefinedShape[2] = FVector(TrianglePositions.X, -TrianglePositions.Y, -TrianglePositions.Z);
	TangentSetup = FProcMeshTangent(0.0f, 1.0f, 0.0f);

	CreateTriangle(DefinedShape[0], DefinedShape[1], DefinedShape[2], TriangleIndexCount, TangentSetup);

	proc_mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}


void Aflat_water::CreateTriangle(FVector TopLeft, FVector BottomLeft, FVector BottomRight, int32& TriIndex, FProcMeshTangent Tangent){

	int32 Point1 = TriIndex++;
	int32 Point2 = TriIndex++;
	int32 Point3 = TriIndex++;

	Vertices.Add(TopLeft);
	Vertices.Add(BottomLeft);
	Vertices.Add(BottomRight);

	Triangles.Add(Point1);
	Triangles.Add(Point2);
	Triangles.Add(Point3);

	FVector ThisNormal = FVector::CrossProduct(TopLeft, BottomRight).GetSafeNormal();

	for (int i = 0; i < 3; i++){
		Normals.Add(ThisNormal);
		Tangents.Add(Tangent);
		Colors.Add(FLinearColor::Green);
		//Colors.Add(FLinearColor(0.0f,1.0f,0.0f));
		
	}
	UVs.Add(FVector2D(0.0f, 1.0f)); // Top Left
	UVs.Add(FVector2D(0.0f, 0.0f)); // Bottom Left
	UVs.Add(FVector2D(1.0f, 0.0f)); // Bottom Right
}
