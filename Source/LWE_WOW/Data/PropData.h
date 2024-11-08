// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PropData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LWE_WOW_API UPropData : public UDataAsset
{
	GENERATED_BODY()
	
	UStaticMesh* Mesh;
};
