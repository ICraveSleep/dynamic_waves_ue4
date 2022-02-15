// Fill out your copyright notice in the Description page of Project Settings.


#include "Waves/Waves.h"

// Sets default values
AWaves::AWaves()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("/Game/meshes/10x10Plane.10x10Plane"));
	wave_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = wave_mesh;
	wave_mesh->SetStaticMesh(BaseMesh.Object);
	
	// test_material = CreateDefaultSubobject<UMaterial>(TEXT("MyMaterial"));
	// UMaterialExpressionConstant3Vector*  BaseColorFactorNode = NewObject<UMaterialExpressionConstant3Vector>();
	// BaseColorFactorNode->Constant = FColor::Red;
	//
	// test_material->BaseColor.Connect(0, BaseColorFactorNode);
	// wave_mesh->SetMaterial(0, test_material);
	
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Meshes/M_waves.M_waves"));
	if (FoundMaterial.Succeeded())
	{
		test_material = FoundMaterial.Object;
	}
	wave_mesh->SetMaterial(0, test_material);
	
}

// Called when the game starts or when spawned
void AWaves::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaves::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
