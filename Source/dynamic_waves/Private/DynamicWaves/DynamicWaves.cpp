// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicWaves/DynamicWaves.h"

#include "Components/BoxComponent.h"

// Sets default values
ADynamicWaves::ADynamicWaves()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ThisSceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = ThisSceneRoot;

	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>("Boat");
	BoatMesh->SetupAttachment(RootComponent);
	
	BoatSimulationMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoatSimulation");
	BoatSimulationMesh->SetupAttachment(RootComponent);

	const ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	const ConstructorHelpers::FObjectFinder<UMaterialInterface> CubeMaterial(TEXT("StaticMesh'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	
	BoatMesh->SetStaticMesh(CubeMesh.Object);
	BoatMesh->SetMaterial(0, CubeMaterial.Object);
	BoatMesh->SetSimulatePhysics(true);
	BoatMesh->SetMassOverrideInKg(NAME_None, 10.0f);
	
	BoatSimulationMesh->SetStaticMesh(CubeMesh.Object);
	BoatSimulationMesh->ToggleVisibility(false);  // Static mesh follows the Boat, but it cannot be used for positions?
	BoatSimulationMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADynamicWaves::BeginPlay()
{
	Super::BeginPlay();
	if(BoatSimulationMesh->GetStaticMesh()->GetRenderData()->LODResources.Num() > 0)
	{
		// SetActorTransform(Boat->GetRelativeTransform(), false, nullptr); // TODO(Sondre): Why must the actor be updated to get correct rotation?????
		//UE_LOG(LogTemp, Warning, TEXT("Actor location: {%f, %f, %f}"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z)
		FPositionVertexBuffer* VertexBuffer = &BoatSimulationMesh->GetStaticMesh()->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer;
		const FIndexArrayView TriangleBuffer = BoatSimulationMesh->GetStaticMesh()->GetRenderData()->LODResources[0].IndexBuffer.GetArrayView();
		MeshHandler = new FMeshHandler(VertexBuffer->GetNumVertices(), TriangleBuffer.Num());	
		// if (VertexBuffer){
		// 	const int32 VertexCount = VertexBuffer->GetNumVertices();
		// 	for(int32 VertexIndex = 0; VertexIndex < VertexCount; VertexIndex++){
		// 		// const FVector WorldSpaceVertexLocation = Boat->GetRelativeLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));
		// 		const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));
		// 	}
		// }
	}
	else
	{
		//TODO add error message
	}
	
}

// Called every frame
void ADynamicWaves::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MeshHandler->PrintMeshInfo();
}


void ADynamicWaves::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(MeshHandler)
	{
		delete MeshHandler;
	}
}
