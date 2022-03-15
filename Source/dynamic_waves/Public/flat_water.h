// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "flat_water.generated.h"

UCLASS()
class DYNAMIC_WAVES_API Aflat_water : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aflat_water();

	UPROPERTY(EditAnywhere)
	FVector TrianglePositions = FVector(100.0f, 100.0f, 100.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
//	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Waves")
	//UStaticMeshComponent* box_mesh;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* proc_mesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* root_secne_component;
	
private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> Colors;
	
	void GenerateMesh();
	void CreateTriangle(FVector TopLeft, FVector BottomLeft, FVector BottomRight, int32& TriIndex, FProcMeshTangent Tangent);
	
};
