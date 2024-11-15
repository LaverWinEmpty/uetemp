// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Shuriken.h"

#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Generic/GenericCharacter.h>

USkillShuriken::USkillShuriken()
{
	Default.Power = 0.1;

	Default.Area     = 1;
	Default.Range    = 1000;
	Default.Speed    = 1000;
	Default.Cooldown = 0.1;

	Target = static_cast<uint8>(ERelationType::HARM);
	Option = static_cast<uint8>(ESkillFlag::IS_NON_TARGET);
}

void USkillShuriken::Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillShuriken::Final(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillShuriken::OnTick(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
	InCaster->Damage(In, InTarget);
}
