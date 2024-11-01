// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Arrow.h"

#include <LWE_WOW/Generic/GenericCharacter.h>

USkillArrow::USkillArrow() : USkillData()
{
	Default.Range = 2400;
	Default.Speed = 10000;

	Default.Cooldown = 5; // 쿨타임

	Target = static_cast<uint8>(ERelationType::HARM);

	// 옵션:  시야 내 객체만 가능 | 회피 가능
	Option = static_cast<uint8>(
		ESkillFlag::LIMIT_VIEW | ESkillFlag::IS_DODGEABLE
	);
}

void USkillArrow::Execute(const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
}

void USkillArrow::Final(const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
	InCaster->Damage(In, InTarget);
	ApplyBuff(InTarget);
}

void USkillArrow::OnTick(const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
}
