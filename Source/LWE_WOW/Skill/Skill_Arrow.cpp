// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Arrow.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>
#include <LWE_WOW/Generic/GenericSkill.h>

USkillArrow::USkillArrow() : USkillData()
{
	Default.Range = 2400;
	Default.Speed = 10000;

	Default.Cooldown = 5; // 쿨타임

	Target = static_cast<uint8>(ERelationType::HARM);

	// 옵션:  시야 내 객체만 가능
	Option = static_cast<uint8>(
		ESkillFlag::LIMIT_VIEW
	);
}

void USkillArrow::Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillArrow::Final(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
	Super::Final(In, InCaster, InTarget, InActor);
	ApplyBuff(InCaster, InTarget, In->Level);
}

void USkillArrow::OnTick(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}
