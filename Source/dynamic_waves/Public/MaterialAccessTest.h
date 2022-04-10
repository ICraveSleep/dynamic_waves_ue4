// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialAccessTest.generated.h"

UCLASS()
class DYNAMIC_WAVES_API AMaterialAccessTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaterialAccessTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *Mesh;
    UMaterialInstanceDynamic* DynamicMaterial;
	UMaterialInterface* Material;
};
