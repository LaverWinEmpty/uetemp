// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Manager/LoadingManager.h>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartMode.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API AStartMode : public AGameModeBase
{
	GENERATED_BODY()
	
	void BeginPlay() {
		Super::BeginPlay();
		ULoadingManager::LoadLevel(this, _T("LobbyMap"));
	}

};
