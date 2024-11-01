// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerManager.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API UPlayerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UPlayerManager* Instance(UObject* InWorldContextObject);

public:
	FName PlayerNameOfDatatableRow;
};
