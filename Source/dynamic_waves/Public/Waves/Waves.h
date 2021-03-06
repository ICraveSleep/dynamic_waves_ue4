// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Waves.generated.h"

UCLASS()
class DYNAMIC_WAVES_API AWaves : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWaves();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Waves")
	UStaticMeshComponent* wave_mesh;
	
	UPROPERTY(EditAnywhere, Category = "Waves")
	UMaterial* test_material;
};
