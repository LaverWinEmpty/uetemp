// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Slash.h"

USkillSlash::USkillSlash() : USkillData()
{
	Default.Range = 600;
	Default.Speed = 0;

	Default.Cooldown = 5; // 쿨타임
	Default.Duration = 1; // 지속 시간
	Default.Interval = 0.2; // 공격시간

	Target = static_cast<uint8>(ERelationType::HARM);

	// 옵션:  시야 내 객체만 가능
	Option = static_cast<uint8>(
		ESkillFlag::LIMIT_VIEW 
	);
}

void USkillSlash::Execute(const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
}

void USkillSlash::Final(const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
}

void USkillSlash::OnTick(const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
	InCaster->Damage(In, InTarget);
	ApplyBuff(InTarget);
}
