// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EnemyInfo.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct LWE_WOW_API FEnemyInfo
{
	GENERATED_BODY()

public:
	FEnemyInfo();
	~FEnemyInfo();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FName AssetTableNowName;
};