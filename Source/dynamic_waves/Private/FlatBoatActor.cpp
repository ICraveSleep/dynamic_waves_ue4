// Fill out your copyright notice in the Description page of Project Settings.


#include "FlatBoatActor.h"

#include "AITypes.h"

// Sets default values
AFlatBoatActor::AFlatBoatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> BoatMesh(TEXT("/Game/Meshes/SM_Boat.SM_Boat"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SimMesh(TEXT("/Game/Meshes/SM_Boat_Tri.SM_Boat_Tri"));	



	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoatMesh(TEXT("/Game/Meshes/SM_Box.SM_Box"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SimMesh(TEXT("/Game/Meshes/SM_Box.SM_Box"));


	// static ConstructorHelpers::FObjectFinder<UStaticMesh> BoatMesh(TEXT("/Game/Meshes/box.box"));
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> SimMesh(TEXT("/Game/Meshes/box.box"));	
	
	
	ThisSceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = ThisSceneRoot;

	Boat = CreateDefaultSubobject<UStaticMeshComponent>("Boat");
	Boat->SetupAttachment(RootComponent);
	Boat->SetStaticMesh(BoatMesh.Object);
	Boat->SetSimulatePhysics(true);
	Boat->SetMassOverrideInKg(NAME_None, 100.0f);
	
	BoatSim = CreateDefaultSubobject<UStaticMeshComponent>("Sim");
	BoatSim->SetupAttachment(Boat);
	BoatSim->SetStaticMesh(SimMesh.Object);
	BoatSim->ToggleVisibility(false);  // Static mesh follows the Boat, but it cannot be used for positions?
	BoatSim->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void AFlatBoatActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AFlatBoatActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Vertices.Reset();

	if(BoatSim->GetStaticMesh()->GetRenderData()->LODResources.Num() > 0)
	{
		SetActorTransform(Boat->GetRelativeTransform(), false, nullptr); // TODO(Sondre): Why must the actor be updated to get correct rotation?????
		//UE_LOG(LogTemp, Warning, TEXT("Actor location: {%f, %f, %f}"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z)
		FPositionVertexBuffer* VertexBuffer = &BoatSim->GetStaticMesh()->GetRenderData()->LODResources[0].VertexBuffers.PositionVertexBuffer;
		const FIndexArrayView TriangleBuffer = BoatSim->GetStaticMesh()->GetRenderData()->LODResources[0].IndexBuffer.GetArrayView();
		
		if (VertexBuffer){
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for(int32 VertexIndex = 0; VertexIndex < VertexCount; VertexIndex++){
				// const FVector WorldSpaceVertexLocation = Boat->GetRelativeLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));
				const FVector WorldSpaceVertexLocation = GetActorLocation() + GetTransform().TransformVector(VertexBuffer->VertexPosition(VertexIndex));
				if(first_run)
				{
					//UE_LOG(LogTemp, Warning, TEXT("VertexSize: %i, TriangleSize: %i"), VertexBuffer->GetNumVertices(), TriangleBuffer.Num())
					//UE_LOG(LogTemp, Warning, TEXT("VertexCount[%i] - TriangleIndex[%i] = %i"), VertexIndex,VertexIndex, TriangleBuffer[VertexIndex]);
					if(TriangleBuffer[VertexIndex] == 635)
					{
					//	UE_LOG(LogTemp, Warning, TEXT("VertexCount[%i] - TriangleIndex[%i]"), VertexIndex, TriangleBuffer[VertexIndex]);
					}
				}
				
				Vertices.Add(WorldSpaceVertexLocation);
				
			}
		}

		if (TriangleBuffer.Num() > 0){
			const int32 TriangleCount = TriangleBuffer.Num();
			for(int32 VertexIndex = 0; VertexIndex < TriangleCount; VertexIndex++){
				if(first_run){
					//UE_LOG(LogTemp, Warning, TEXT("TriangleSize: %i, Triangle[%i]: %i"), TriangleBuffer.Num(), VertexIndex, TriangleBuffer[VertexIndex])
					}
				}
			}
		
		

		for(int i = 0; i < Vertices.Num() - 1; i++){
			// DrawDebugPoint(this->GetWorld(), Vertices[0], 15.0f, FColor(0, 0, 0),false, 0.0f, 0);	
			// DrawDebugPoint(this->GetWorld(), Vertices[1], 15.0f, FColor(0, 255, 0),false, 0.0f);	
			// DrawDebugPoint(this->GetWorld(), Vertices[2], 15.0f, FColor(255, 255, 0),false, 0.0f);	
			// DrawDebugPoint(this->GetWorld(), Vertices[3], 15.0f, FColor(0, 0, 255),false, 0.0f);
			
			DrawDebugPoint(this->GetWorld(), Vertices[i], 15.0f, FColor(255, 0, 0),false, 0.0f);
		}

		// FBodyInstance *Body = Boat->GetBodyInstance();
		// DrawDebugSphere(GetWorld(), Body->GetCOMPosition(), 100.f, 32, FColor::Yellow);
		
		// DrawDebugSphere(GetWorld(), Boat->GetCenterOfMass(), 100.f, 32, FColor::Yellow);
		
		for(int i = 1; i < TriangleBuffer.Num(); i=i+3){
			float x_sum = Vertices[TriangleBuffer[i-1]].X + Vertices[TriangleBuffer[i]].X + Vertices[TriangleBuffer[i+1]].X;
			float y_sum = Vertices[TriangleBuffer[i-1]].Y + Vertices[TriangleBuffer[i]].Y + Vertices[TriangleBuffer[i+1]].Y;
			float z_sum = Vertices[TriangleBuffer[i-1]].Z + Vertices[TriangleBuffer[i]].Z + Vertices[TriangleBuffer[i+1]].Z;
			FVector center = {x_sum/3.0f, y_sum/3.0f, z_sum/3.0f};
			// if(Vertices[TriangleBuffer[i-1]].Z < 0)
			// if(Vertices[TriangleBuffer[i-1]].Z < 0)

			
			float WorldTime = this->GetWorld()->GetTimeSeconds();
			FVector2D WaveDirection = {1.0f, 0.0f};
			WaveDirection.Normalize();
			FVector2D BoatPosXY = {center.X, center.Y};
			float TestHeight = FVector2D::DotProduct(BoatPosXY, WaveDirection);
			float WaveHeight = 150.0f*sin(0.01*(PI*2/48)*TestHeight + WorldTime);

			if(first_run)
			{
				// UE_LOG(LogTemp, Warning, TEXT("Center: %f - Height: %f"), center.Z, WaveHeight);
			}
			
			if(center.Z <= 0)
			{
				FVector p1 = Vertices[TriangleBuffer[i-1]];
				FVector p2 = Vertices[TriangleBuffer[i]];
				FVector p3 = Vertices[TriangleBuffer[i+1]];
				FVector normal = FVector::CrossProduct(p3-p1, p2-p1);
				if(DrawNormals)
				{
					DrawDebugLine(this->GetWorld(), center, normal, FColor{0,0,255}, false, 0.0f, 0, 2.0f);
				}
				DrawDebugPoint(this->GetWorld(), center, 15.0f, FColor(0, 0, 0),false, 0.0f);
				DrawDebugLine(this->GetWorld(), Vertices[TriangleBuffer[i-1]], Vertices[TriangleBuffer[i]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
				DrawDebugLine(this->GetWorld(), Vertices[TriangleBuffer[i]], Vertices[TriangleBuffer[i+1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);
				DrawDebugLine(this->GetWorld(), Vertices[TriangleBuffer[i+1]], Vertices[TriangleBuffer[i-1]], FColor{255,255,255}, false, 0.0f, 0, 2.0f);

				float a = FVector::Distance(p2, p1);
				float b = FVector::Distance(p3, p1);
				float Dot_Product = FVector::DotProduct(p2-p1, p3-p1) / (a*b);
				float w = acos(Dot_Product);
				
				if(w > 0)
				{
					//UE_LOG(LogTemp, Warning, TEXT("w: %f, dot: %f"), w, Dot_Product);
				}
				
				if(w < 0){
					UE_LOG(LogTemp, Warning, TEXT("Calculated angle that was negative: w=%f"), w);
					w = 0;
				}
				float AreaTriangle = 0.5*a*b*sin(w); // is in cm^2
				AreaTriangle = AreaTriangle*0.00001; // Convert to m^2
				if(first_run){
					//UE_LOG(LogTemp, Warning, TEXT("Area[%i]: %f[cm^2]"), i, AreaTriangle);
				}
				
				
				FVector Force =  1027.0f * (center.Z)*0.01f * AreaTriangle * normal*0.01f;
				
				Force.X = 0.0f;
				Force.Y = 0.0f;
				// UE_LOG(LogTemp, Warning, TEXT("Area[%i]: %f[cm^2]"), i, AreaTriangle);
				// Boat->AddForceAtLocation({0.0f, 0.0f, Force.Z}, center, NAME_None);
				if(EnableForces)
				{
					Boat->AddForceAtLocation(Force, center, NAME_None);	
				}
				
				// Boat->AddForceAtLocation({0.0f, 0.0f, 400.0f}, center, NAME_None);
				
				FVector drag = {0.0f, 0.0f, 0.0f};
				float C = 8.8f;
				float R = 0;
				// float R = 0.5f*TotalSubmerged_Area*1027.0f*C*Boat->GetPhysicsLinearVelocity().Z*Boat->GetPhysicsLinearVelocity().Z;
				FVector V = Boat->GetComponentVelocity();
				V = V*0.01f;
				FVector W = Boat->GetPhysicsAngularVelocityInRadians();
				FVector R_BA = center - Boat->GetCenterOfMass();
				//UE_LOG(LogTemp, Warning, TEXT("Center of mass: {%f, %f, %f}"), Boat->GetCenterOfMass().X, Boat->GetCenterOfMass().Y, Boat->GetCenterOfMass().Z);
				FVector V_A =  V + FVector::CrossProduct(W, R_BA)*0.01f;
				
				// UE_LOG(LogTemp, Warning, TEXT("Vel: %f"), V);
				if(V_A.Z < 0)
				{
					 R = 0.5f*AreaTriangle*1027.0f*C*V_A.Z*V_A.Z;	
				}
				else
				{
					R = -0.5f*AreaTriangle*1027.0f*C*V_A.Z*V_A.Z;
				}
		
				drag.Z = R;
				if(EnableForces)
				{
					Boat->AddForceAtLocation(drag, center, NAME_None);	
				}
				
			}
		}

		
		
		if(first_run){
			first_run = false;
		}
	}
}

