// Fill out your copyright notice in the Description page of Project Settings.


#include "Waves/WaveInterractor.h"

// Sets default values
AWaveInterractor::AWaveInterractor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("/Game/meshes/box.box"));
	box_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boxxy"));
	
	RootComponent = box_mesh;
	box_mesh->SetStaticMesh(BaseMesh.Object);

}

// Called when the game starts or when spawned
void AWaveInterractor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaveInterractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D direction = {1.0, 1.0};
	FVector world_position = box_mesh->GetComponentLocation();
	FVector2D world_pos = {world_position.X, world_position.Y};
	direction.Normalize();
	
	float time = this->GetWorld()->GetTimeSeconds();
	
	float height =30*sin( 0.01*((PI*2)/3) + time);
	// box_mesh->SetRelativeLocation({0.0f,0.0f,height});
	FVector position = box_mesh->GetComponentLocation();
	FVector velocity = box_mesh->GetPhysicsLinearVelocity();
	if(position.Z <= height){
		float f_b = 2.5f * 9.81f * 1.0f * (height - position.Z);
		float f_d = 1.0f * 1.0f * 0.01f * velocity.Z * velocity.Z / 2.0f;
		if(velocity.Z < 0.0f)
		{
			box_mesh->AddForce({0,0,f_b+f_d});	
		}
		else
		{
			box_mesh->AddForce({0,0,f_b-f_d});	
		}
		// box_mesh->AddForce({0,0,f_b});	
			
	}


}

