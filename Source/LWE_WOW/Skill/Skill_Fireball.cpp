// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Fireball.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>

USkillFireball::USkillFireball() : USkillData()
{
	Default.Range = 1000;
	Default.Speed = 1000;

	Default.Cooldown = 5; // 쿨타임

	Default.Charge = 1; // 캐스팅 시간

	Default.Duration = 3;   // 3초간 틱
	Default.Interval = 0.5; // 0.5초마다

	Target = static_cast<uint8>(ERelationType::HARM);

	// 옵션:  시야 내 객체만 가능 | 유도
	Option = static_cast<uint8>(
		ESkillFlag::LIMIT_VIEW | ESkillFlag::IS_GUIDED
	);
}

void USkillFireball::Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillFireball::Final(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillFireball::OnTick(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
	Super::OnTick(In, InCaster, InTarget, InActor);

	InCaster->Damage(In, InTarget);
}
