// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Generic/GenericInput.h>
#include <LWE_WOW/Generic/GenericCharacter.h>

#include "CoreMinimal.h"

#include "TestInput.generated.h"

/**
 *
 */
UCLASS()
class LWE_WOW_API ATestInput : public AGenericInput
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

public:
	virtual void PostMouseLeftClick (const TOptional<FHitResult>&) override;
	virtual void PostMouseRightClick(const TOptional<FHitResult>&) override;


};
