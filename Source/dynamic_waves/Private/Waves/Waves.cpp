// Fill out your copyright notice in the Description page of Project Settings.


#include "Waves/Waves.h"

// Sets default values
AWaves::AWaves()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called to bind functionality to input
void AWaves::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

