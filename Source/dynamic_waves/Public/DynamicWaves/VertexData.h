// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct FVertexData
{
 float SurfaceDistance;
 int32_t Index;  // Index used when constructing the custom sub triangles
 FVector WorldVertexLocation;
};
