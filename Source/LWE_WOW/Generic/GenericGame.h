// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GenericGame.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API UGenericGame : public UGameInstance
{
	GENERATED_BODY()
	

public:
	FName PlayerNameInDatatableRow;
};
