// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"

// Add default functionality here for any IClickable functions that are not pure virtual.

ETargetType IInteractable::GetType() const
{
	// 변수 또는 객체에 따라 다른 타입을 반환합니다.
	// 기본값은 NPC 입니다.
	return ETargetType::NPC;
}

ERelationType IInteractable::GetRelation(IInteractable* InArg) const
{
	if      (this == InArg)                                     return ERelationType::SELF; // 자기 자신 체크
	else if ((GetTeam() & InArg->GetTeam()) != ETeamType::NONE) return ERelationType::HELP; // 팀 플래그가 같다면 아군
	else                                                        return ERelationType::HARM; // 적
}

bool IInteractable::IsPlayer() const
{
	return GetType() == ETargetType::PLAYER;
}

ETeamType IInteractable::GetTeam() const
{
	return ETeamType::NONE;
}

void IInteractable::OnTargetting()
{
	// 타겟 객체를 통해 타겟팅 될 경우
	// 타겟 된 객체에게 알림 등을 전달하기 위해 사용합니다.
	// NPC의 경우 대화창을 여는 등의 행동도 이 메소드를 사용합니다.
	// 기본적으로는 사용되지 않아 빈 메소드입니다.
}

void IInteractable::SetNeutral(ETeamType& Out)
{
	Out = ETeamType::A | ETeamType::B;
}

void IInteractable::SetEnemy(ETeamType& Out)
{
	Out = ETeamType::NONE;
}
