// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "CopyMeshFromStatic.generated.h"

UCLASS()
class DYNAMIC_WAVES_API ACopyMeshFromStatic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACopyMeshFromStatic();

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "MeshCopy")
	UStaticMeshComponent* box_mesh;
	UStaticMeshComponent* test_mesh;

	void GenerateMesh();
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "MeshCopy")
	UProceduralMeshComponent* proc_mesh;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "MeshCopy")
	USceneComponent* root_secne_component;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "MeshCopy")
	UStaticMesh* static_mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> Colors;

};
