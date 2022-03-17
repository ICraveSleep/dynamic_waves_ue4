// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatActor.h"

// Sets default values
ABoatActor::ABoatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoatMesh(TEXT("/Game/Meshes/SM_Boat.SM_Boat"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SimMesh(TEXT("/Game/Meshes/SM_Boat_Tri.SM_Boat_Tri"));
	
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
		UE_LOG(LogTemp, Warning, TEXT("Actor location: {%f, %f, %f}"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z)
		FPositionVertexBuffer* VertexBuffer = &BoatSim->GetStaticMesh()->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer;
		if (VertexBuffer){
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for(int32 VertexIndex = 0; VertexIndex < VertexCount; VertexIndex++){
				// const FVector WorldSpaceVertexLocation = Boat->GetRelativeLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));
				const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));

				Vertices.Add(WorldSpaceVertexLocation);
				
			}
		}
		for(int i = 0; i < Vertices.Num(); i++)
		{
			DrawDebugPoint(this->GetWorld(), Vertices[i], 20.0f, FColor(255, 0, 0),false, 0.0f);
		}	
	}
	
}

