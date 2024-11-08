// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill_Area.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>

USkillArea::USkillArea() : USkillData()
{
	Default.Range = 0; // 객체가 시전하는 기술이 아님
	Default.Speed = 0; // 이동하는 객체가 아님

	Default.Duration = 5; // 지속 시간
	Default.Interval = 0.1; // 공격시간

	Target = static_cast<uint8>(ERelationType::HARM);

	Option = static_cast<uint8>(ESkillFlag::IS_NON_TARGET); // 논타겟 스킬 (장판)
}

void USkillArea::Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillArea::Final(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillArea::OnTick(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
	Super::OnTick(In, InCaster, InTarget, InActor);
	InCaster->Damage(In, InTarget);
}
