// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PersistentLevelMode.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API APersistentLevelMode : public AGameModeBase
{
	GENERATED_BODY()

	void StartPlay() {
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FName FirstMapName;
};
