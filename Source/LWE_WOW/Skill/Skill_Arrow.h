// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Data/SkillData.h>

#include "CoreMinimal.h"
#include "Skill_Arrow.generated.h"

/**
 *  화살 쏘는 스킬
 */
UCLASS()
class LWE_WOW_API USkillArrow : public USkillData
{
	GENERATED_BODY()

	USkillArrow();
	
	virtual void Execute(UGenericSkill*, AGenericCharacter*, AGenericCharacter*, AGenericEffect*) const override;
	virtual void Final  (UGenericSkill*, AGenericCharacter*, AGenericCharacter*, AGenericEffect*) const override;
	virtual void OnTick (UGenericSkill*, AGenericCharacter*, AGenericCharacter*, AGenericEffect*) const override;
};
