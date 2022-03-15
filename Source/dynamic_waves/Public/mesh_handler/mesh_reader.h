// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "mesh_reader.generated.h"

UCLASS()
class DYNAMIC_WAVES_API Amesh_reader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Amesh_reader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* box_mesh;

};
