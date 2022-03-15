// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyMeshFromStatic.h"

// Sets default values
ACopyMeshFromStatic::ACopyMeshFromStatic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root_secne_component = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root_secne_component;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("/Game/meshes/box.box"));
	static_mesh = BaseMesh.Object;

//	box_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boxxy"));
//	box_mesh->SetupAttachment(RootComponent);
//	box_mesh->SetStaticMesh(BaseMesh.Object);

	proc_mesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProcedualMesh");
	proc_mesh->SetupAttachment(RootComponent);
	// static_mesh = box_mesh->GetStaticMesh();

	

}

// Called when the game starts or when spawned
void ACopyMeshFromStatic::BeginPlay()
{
	Super::BeginPlay();
//	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(box_mesh, 0, proc_mesh, false);

	
	if(static_mesh->GetRenderData()->LODResources.Num() > 0){
		FPositionVertexBuffer* VertexBuffer = &static_mesh->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer;
		if(VertexBuffer){
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for(int32 index = 0; index < VertexCount; index++){
				const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(index));
				Vertices.Add(WorldSpaceVertexLocation);
			}
		}
	}

	GenerateMesh();
	
	
}

// Called every frame
void ACopyMeshFromStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACopyMeshFromStatic::GenerateMesh(){
	
	
	FProcMeshTangent TangentSetup = FProcMeshTangent(0.0f, 1.0f, 0.0f);
	FVector A = Vertices[0];
	FVector B = Vertices[1];
	FVector C = Vertices[2];
	UE_LOG(LogTemp, Warning, TEXT("0: %f.%f.%f, 1: %f.%f.%f, 2: %f.%f.%f"), A.X, A.Y, A.Z, B.X, B.Y, B.Z, C.X, C.Y, C.Z);
	int32 Point1 = 0;
	int32 Point2 = 1;
	int32 Point3 = 2;

	Triangles.Add(Point1);
	Triangles.Add(Point2);
	Triangles.Add(Point3);

	FVector ThisNormal = FVector::CrossProduct(Vertices[0], Vertices[2]).GetSafeNormal();

	for (int i = 0; i < 3; i++){
		Normals.Add(ThisNormal);
		Tangents.Add(TangentSetup);
		Colors.Add(FLinearColor::Green);
		//Colors.Add(FLinearColor(0.0f,1.0f,0.0f));
	}
	UVs.Add(FVector2D(0.0f, 1.0f)); // Top Left
	UVs.Add(FVector2D(0.0f, 0.0f)); // Bottom Left
	UVs.Add(FVector2D(1.0f, 0.0f)); // Bottom Right
	Vertices.Reset();
	Vertices.Add(A);
	Vertices.Add(B);
	Vertices.Add(C);
	proc_mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

