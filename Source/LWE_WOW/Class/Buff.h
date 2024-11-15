// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Data/BuffData.h>

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"

class AGenericCharacter;

// 함수 객체를 사용하는 일종의 헬퍼 클래스입니다.
class LWE_WOW_API CBuff
{
public:
	CBuff(AGenericCharacter*);
	~CBuff();

public:
	struct Applied {
		UBuffData*       Data;  // 이펙트 정보입니다.
		int              Level; // 해당 이펙트 레벨입니다.
		FBuffInfo        Info;  // 해당 이펙트 정보입니다. (데이터에서 계산됨)
		FBuffInfo::State State; // 해당 이펙트 상태입니다.
	};

public:
	void Add(AGenericCharacter*, UBuffData*, int InLevel);
	void OnTick(float DeltaTime);

private:
	AGenericCharacter*               m_Parent;  // 이펙트를 적용할 객체입니다.
	AGenericCharacter*               m_Caster;  // 이펙트를 적용한 객체입니다.
	TMap<FName, TSharedPtr<Applied>> m_Applied; // 해당 객체가 적용중인 이펙트입니다.
};
