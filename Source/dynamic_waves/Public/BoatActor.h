// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "BoatActor.generated.h"

UCLASS()
class DYNAMIC_WAVES_API ABoatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoatActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boat")
	USceneComponent *ThisSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boat")
	UStaticMeshComponent* Boat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boat")
	UStaticMeshComponent* BoatSim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boat")
	bool DrawNormals = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	bool first_run = true;
};
