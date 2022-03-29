// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct FVertexData
{
 float SurfaceDistance;
 int32_t Index;
 FVector WorldVertexLocation;
};

struct FTrianglePrimitive  //TODO(Sondre): See if it can be removed and rather use a list of FVertexData
{
 FVertexData A;
 FVertexData B;
 FVertexData C;
};