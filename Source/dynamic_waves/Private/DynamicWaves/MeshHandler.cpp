// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicWaves/MeshHandler.h"

FMeshHandler::FMeshHandler(uint32_t VertexSizeIn, uint32_t TriangleSizeIn)
{
	
	TriangleSize = TriangleSizeIn;
	VertexSize = VertexSizeIn;
}

FMeshHandler::~FMeshHandler()
{
}

void FMeshHandler::PrintMeshInfo()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow,
	                                 FString::Printf(
		                                 TEXT("VertexSize: %i, TriangleSize: %i"), VertexSize, TriangleSize));
	UE_LOG(LogTemp, Warning, TEXT("Hello"));
}
