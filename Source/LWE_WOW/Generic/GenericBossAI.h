// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericBossAI.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API AGenericBossAI : public AAIController
{
	GENERATED_BODY()
	
public:
	AGenericBossAI();

public:
	virtual void OnPossess(APawn* In) override;

public:
	virtual void Initialize();
};
