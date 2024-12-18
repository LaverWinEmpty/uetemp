// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/SkillInfo.h>
#include <LWE_WOW/UI/SlotInfo.h>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GenericSkill.generated.h"

class USkillData;

/**
 * 
 */
UCLASS()
class LWE_WOW_API UGenericSkill : public USlot
{
	GENERATED_BODY()

public:
	UPROPERTY()
	USkillData* Data;
	FSkillInfo  Info;
	int         Level;
	float       Cooldown; // 현재 쿨타임

public:
	void SetData(USkillData*, int InLevel);
	void SetLevel(int);

public:
	operator bool() { return Data != nullptr; }	
};
