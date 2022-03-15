// Fill out your copyright notice in the Description page of Project Settings.


#include "mesh_handler/mesh_reader.h"

// Sets default values
Amesh_reader::Amesh_reader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("/Game/meshes/box.box"));
	box_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boxxy"));
	
	RootComponent = box_mesh;
	box_mesh->SetStaticMesh(BaseMesh.Object);
}

// Called when the game starts or when spawned
void Amesh_reader::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Amesh_reader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (box_mesh->GetStaticMesh()->GetRenderData()->LODResources.Num() > 0)
	{
		TArray<FVector> vertices = TArray<FVector>();
		
		FPositionVertexBuffer* VertexBuffer = &box_mesh->GetStaticMesh()->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer;

		int triangles = box_mesh->GetStaticMesh()->GetRenderData()->LODResources[0].GetNumTriangles();
		int vertecies = box_mesh->GetStaticMesh()->GetRenderData()->LODResources[0].GetNumVertices();
		
		UE_LOG(LogTemp, Warning, TEXT("Number of triangles: %i, and vertecies: %i"), triangles, vertecies);
		
		if (VertexBuffer)
		{
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for (int32 Index = 0; Index < VertexCount; Index++) {
				//This is in the Static Mesh Actor Class, so it is location and tranform of the SMActor
				const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(Index));
				//add to output FVector array
				vertices.Add(WorldSpaceVertexLocation);
			}
			for(int i = 0; i < vertices.Num(); i++) {
				DrawDebugPoint(this->GetWorld(), vertices[i], 20.0f, FColor(124, 124, 124),false, 0.0f);
				
				DrawDebugPoint(this->GetWorld(), vertices[0], 20.0f, FColor(0, 0, 0),false, 0.0f);	
				DrawDebugPoint(this->GetWorld(), vertices[1], 20.0f, FColor(0, 255, 0),false, 0.0f);	
				DrawDebugPoint(this->GetWorld(), vertices[2], 20.0f, FColor(255, 255, 0),false, 0.0f);	
				DrawDebugPoint(this->GetWorld(), vertices[3], 20.0f, FColor(255, 0, 0),false, 0.0f);	
				
				
						
				
				
				DrawDebugLine(this->GetWorld(), vertices[1], vertices[2], FColor(255, 0, 255), false, 0.0f, 0.0f, 1.0f);	
				DrawDebugLine(this->GetWorld(), vertices[1+3], vertices[2+3], FColor(255, 0, 255), false, 0.0f, 0.0f, 1.0f);	
				DrawDebugLine(this->GetWorld(), vertices[1+6], vertices[2+6], FColor(255, 0, 255), false, 0.0f, 0.0f, 1.0f);	
				DrawDebugLine(this->GetWorld(), vertices[1+9+1], vertices[2+9+1], FColor(255, 0, 255), false, 0.0f, 0.0f, 1.0f);	
				DrawDebugLine(this->GetWorld(), vertices[1+12+2], vertices[2+12+2], FColor(255, 0, 255), false, 0.0f, 0.0f, 1.0f);	
				DrawDebugLine(this->GetWorld(), vertices[1+15+4], vertices[2+15+4], FColor(255, 0, 255), false, 0.0f, 0.0f, 1.0f);	
			}
			
				
		}
	}

	
}
