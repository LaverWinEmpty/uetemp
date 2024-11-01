// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GenericMode.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API AGenericMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Camera"); float CameraRangeHorizontal = 360;
	UPROPERTY(EditAnywhere, Category = "Camera"); float CameraRangeVertical   = 120;
	UPROPERTY(EditAnywhere, Category = "Camera"); float ZoomMin               = 0;
	UPROPERTY(EditAnywhere, Category = "Camera"); float ZoomMax               = 1000;
};
