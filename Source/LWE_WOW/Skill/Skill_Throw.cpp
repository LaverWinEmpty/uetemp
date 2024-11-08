// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill_Throw.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>
#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Skill/Skill_Area.h>

USkillThrow::USkillThrow() : USkillData()
{
	Default.Range = 2000;
	Default.Speed = 0;

	Default.Cooldown = 5; // 쿨타임
	Default.Duration = 0; // 즉시 시전

	Target = static_cast<uint8>(ERelationType::HARM);
}

void USkillThrow::Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
	// 확인용
	if (!In->Data->Next.Num()) {
		return;
	}

	// 데이터 1개 가져옴
	USkillArea* AreaSkillData = In->Data->Next[0]->GetDefaultObject<USkillArea>();
	check(AreaSkillData);

	// 다른 스킬 생성 위치 지정
	FVector Location = InActor->GetActorLocation() + InActor->GetActorForwardVector() * 50 * In->Info.Scale;

	// 다른 스킬로 파생: 가져온 데이터를 기반으로, 레벨은 현재 스킬 레벨을 따릅니다.
	AGenericEffect* Effect = AGenericEffect::Spawn(AreaSkillData, In->Level, _T("Effect"), InCaster, InTarget, Location);
}

void USkillThrow::Final(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}

void USkillThrow::OnTick(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const
{
}
