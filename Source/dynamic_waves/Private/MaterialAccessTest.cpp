// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialAccessTest.h"

// Sets default values
AMaterialAccessTest::AMaterialAccessTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	
}

// Called when the game starts or when spawned
void AMaterialAccessTest::BeginPlay()
{
	Super::BeginPlay();
	Material = Mesh->GetMaterial(0);
	float WaveHeight = 30*sin(GetWorld()->TimeSeconds);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	Mesh->SetMaterial(0, DynamicMaterial);
	DynamicMaterial->SetScalarParameterValue(TEXT("WaveHeight"), 45.0f);
	

}

// Called every frame
void AMaterialAccessTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// float WaveHeight = 30*sin(this->GetWorld()->TimeSeconds);
	// UE_LOG(LogTemp, Warning, TEXT("%f"), WaveHeight);
	// DynamicMaterial->SetScalarParameterValue(TEXT("WaveHeigth"), WaveHeight);
}

