// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/SkillInfo.h>

#include "CoreMinimal.h"

class AGenericCharacter;
class AGenericInput;
class USkillData;
class UAnimMontage;
class IInteractable;
class UGenericSkill;

/**
 * 
 */
class LWE_WOW_API CAct
{
public:
	enum EResult {
		ACT_SUCCEED,
		ACT_ACTING,
		ACT_DIED,
		ACT_COOLDOWN,
		ACT_OUT_OF_RANGE,
		ACT_OUT_OF_VIEW,
		ACT_CAN_NOT_MOVING,
		ACT_TARGET_NOT_EXISTS,
		ACT_TARGET_IS_INVALID,
		ACT_TARGET_IS_DIED,
	};

public:
	static bool IsOK(EResult);

public:
	static const TCHAR* const GetMessage(EResult Result) {
		switch (Result)
		{
			case ACT_OUT_OF_RANGE:      return _T("대상이 사거리를 벗어났습니다.");
			case ACT_OUT_OF_VIEW:       return _T("대상이 시야에 없습니다.");
			case ACT_COOLDOWN:          return _T("아직 사용할 수 없습니다.");
			case ACT_CAN_NOT_MOVING:    return _T("이동 중엔 사용할 수 없습니다.");
			case ACT_TARGET_NOT_EXISTS: return _T("대상이 없습니다.");
			case ACT_TARGET_IS_INVALID: return _T("시전할 수 없는 대상입니다.");
			case ACT_TARGET_IS_DIED:    return _T("대상이 이미 죽었습니다.");
			case ACT_DIED:              return _T("죽었습니다.");


			case ACT_SUCCEED:
			case ACT_ACTING:
				return _T("");

			default:
				check(false);
				return nullptr;
		}
	}

public:
	CAct(AGenericCharacter*);
	~CAct();

public:
	EResult Start(UGenericSkill*);

public:
	void Cancel();
	
public:
	// 부모의 Tick에서 실행합니다.
	void OnTick(float);

public:
	void ChargeMotionPlay();

public:
	void OnNotify();

public:
	operator bool() { return m_Current != nullptr; }

public:
	UAnimMontage* ChargeMotion;
	UAnimMontage* ExecuteMotion;

public:
	UGenericSkill* Notify;

private:
	UGenericSkill*     m_Current;
	AGenericCharacter* m_Parent;
	AGenericCharacter* m_Target;
	float              m_Charge = 0;
};
