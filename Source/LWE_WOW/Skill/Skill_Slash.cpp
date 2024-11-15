// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill_Slash.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>

USkillSlash::USkillSlash() : USkillData()
{
	Default.Range = 600;
	Default.Speed = 0;

	Default.Cooldown = 5; // 쿨타임
	Default.Duration = 1; // 지속 시간
	Default.Interval = 0.2; // 공격시간

	Target = static_cast<uint8>(ERelationType::HARM);
}

void USkillSlash::Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillSlash::Final(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillSlash::OnTick(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
	Super::OnTick(In, InCaster, InTarget, InActor);
	InCaster->Damage(In, InTarget);
}
