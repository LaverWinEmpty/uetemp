// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LWE_WOW/Data/SkillData.h"
#include "Skill_Shuriken.generated.h"

// 수리검 던져서 상태이상

UCLASS()
class LWE_WOW_API USkillShuriken : public USkillData
{
	GENERATED_BODY()
	
public:
	USkillShuriken();

public:
	virtual void Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const;
	virtual void Final  (UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const;
	virtual void OnTick (UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const;
};
