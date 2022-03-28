// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshHandler.h"
#include "DynamicWaves.generated.h"

UCLASS()
class DYNAMIC_WAVES_API ADynamicWaves : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicWaves();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boat")
	USceneComponent *ThisSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boat")
	UStaticMeshComponent* BoatMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boat")
	UStaticMeshComponent* BoatSimulationMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boat")
	bool DrawNormals = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boat")
	bool EnableForces = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FMeshHandler* MeshHandler;
	FPositionVertexBuffer* VertexBuffer;
	FIndexArrayView TriangleBuffer;
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
};
