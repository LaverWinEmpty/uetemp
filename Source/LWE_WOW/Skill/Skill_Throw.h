// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LWE_WOW/Data/SkillData.h"
#include "Skill_Throw.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API USkillThrow : public USkillData
{
	GENERATED_BODY()

	USkillThrow();

	virtual void Execute(UGenericSkill*, AGenericCharacter*, AGenericCharacter*, AGenericEffect*) const override;
	virtual void Final  (UGenericSkill*, AGenericCharacter*, AGenericCharacter*, AGenericEffect*) const override;
	virtual void OnTick (UGenericSkill*, AGenericCharacter*, AGenericCharacter*, AGenericEffect*) const override;
};
