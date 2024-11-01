// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Data/BuffData.h>

#include "CoreMinimal.h"
#include "Debuff_Blood.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API UDebuffBlood : public UBuffData
{
	GENERATED_BODY()

public:
	virtual void OnBegin(AGenericCharacter* InTarget) const;
	virtual void OnTick(AGenericCharacter* InTarget) const;
	virtual void OnEnd(AGenericCharacter* InTarget) const;
};
