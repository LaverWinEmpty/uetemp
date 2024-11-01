// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/SkillInfo.h>

#include "CoreMinimal.h"

class AGenericSckill;
class AGenericCharacter;
class AGenericInput;
class USkillData;
class UAnimMontage;
class IInteractable;

/**
 * 
 */
class LWE_WOW_API CAct
{
public:
	struct Skill {
		USkillData* Data;
		FSkillInfo  Info;
		int         Level;

		operator bool() { return Data != nullptr; }
	};

public:
	CAct(AGenericCharacter*);
	~CAct();

public:
	// return: 시전 중인 행동이 있다면 false를 반환합니다
	bool Start(Skill*);

public:
	// 부모의 Tick에서 실행합니다.
	void OnTick(float);

public:
	UAnimMontage* ChargeMotion;
	UAnimMontage* ExecuteMotion;

private:
	Skill*             m_Current;
	AGenericCharacter* m_Parent;
	AGenericCharacter* m_Target;
	float              m_Charge = 0;
};
