// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatActor.h"

// Sets default values
ABoatActor::ABoatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoatMesh(TEXT("/Game/Meshes/SM_Boat.SM_Boat"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SimMesh(TEXT("/Game/Meshes/SM_Boat_Tri.SM_Boat_Tri"));

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> BoatMesh(TEXT("/Game/Meshes/box.box"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SimMesh(TEXT("/Game/Meshes/box.box"));
	
	ThisSceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = ThisSceneRoot;

	Boat = CreateDefaultSubobject<UStaticMeshComponent>("Boat");
	Boat->SetupAttachment(RootComponent);
	Boat->SetStaticMesh(BoatMesh.Object);
	Boat->SetSimulatePhysics(true);
	Boat->SetMassOverrideInKg(NAME_None, 100.0f);
	
	BoatSim = CreateDefaultSubobject<UStaticMeshComponent>("Sim");
	BoatSim->SetupAttachment(Boat);
	BoatSim->SetStaticMesh(SimMesh.Object);
	BoatSim->ToggleVisibility(false);  // Static mesh follows the Boat, but it cannot be used for positions?
	BoatSim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called when the game starts or when spawned
void ABoatActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Vertices.Reset();

	if(BoatSim->GetStaticMesh()->GetRenderData()->LODResources.Num() > 0)
	{
		SetActorTransform(Boat->GetRelativeTransform(), false, nullptr); // TODO(Sondre): Why must the actor be updated to get correct rotation?????
		//UE_LOG(LogTemp, Warning, TEXT("Actor location: {%f, %f, %f}"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z)
		FPositionVertexBuffer* VertexBuffer = &BoatSim->GetStaticMesh()->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer;
		const FIndexArrayView TriangleBuffer = BoatSim->GetStaticMesh()->GetRenderData()->LODResources[0].IndexBuffer.GetArrayView();
		
		if (VertexBuffer){
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for(int32 VertexIndex = 0; VertexIndex < VertexCount; VertexIndex++){
				// const FVector WorldSpaceVertexLocation = Boat->GetRelativeLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));
				const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));
				if(first_run)
				{
					UE_LOG(LogTemp, Warning, TEXT("VertexSize: %i, TriangleSize: %i"), VertexBuffer->GetNumVertices(), TriangleBuffer.Num())
					UE_LOG(LogTemp, Warning, TEXT("VertexCount[%i] - TriangleIndex[%i] = %i"), VertexIndex,VertexIndex, TriangleBuffer[VertexIndex]);
					if(TriangleBuffer[VertexIndex] == 635)
					{
					//	UE_LOG(LogTemp, Warning, TEXT("VertexCount[%i] - TriangleIndex[%i]"), VertexIndex, TriangleBuffer[VertexIndex]);
					}
				}

				Vertices.Add(WorldSpaceVertexLocation);
				
			}
		}

		if (TriangleBuffer.Num() > 0){
			const int32 TriangleCount = TriangleBuffer.Num();
			for(int32 VertexIndex = 0; VertexIndex < TriangleCount; VertexIndex++){
				if(first_run){
					UE_LOG(LogTemp, Warning, TEXT("TriangleSize: %i, Triangle[%i]: %i"), TriangleBuffer.Num(), VertexIndex, TriangleBuffer[VertexIndex])
					}
				}
			}
		

		
		if(first_run){
			first_run = false;
		}

		for(int i = 0; i < Vertices.Num() - 1; i++){
			DrawDebugPoint(this->GetWorld(), Vertices[0], 20.0f, FColor(0, 0, 0),false, 0.0f, 0);	
			DrawDebugPoint(this->GetWorld(), Vertices[1], 20.0f, FColor(0, 255, 0),false, 0.0f);	
			DrawDebugPoint(this->GetWorld(), Vertices[2], 20.0f, FColor(255, 255, 0),false, 0.0f);	
			DrawDebugPoint(this->GetWorld(), Vertices[3], 20.0f, FColor(0, 0, 255),false, 0.0f);
			DrawDebugPoint(this->GetWorld(), Vertices[i], 20.0f, FColor(255, 0, 0),false, 0.0f);
		}
		
		for(int i = 1; i < TriangleBuffer.Num(); i=i+3){
			if(Vertices[TriangleBuffer[i-1]].Z < 0)
			{
				DrawDebugLine(this->GetWorld(), Vertices[TriangleBuffer[i-1]], Vertices[TriangleBuffer[i]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
				DrawDebugLine(this->GetWorld(), Vertices[TriangleBuffer[i]], Vertices[TriangleBuffer[i+1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
				DrawDebugLine(this->GetWorld(), Vertices[TriangleBuffer[i+1]], Vertices[TriangleBuffer[i-1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);	
			}
		}
	}
}


