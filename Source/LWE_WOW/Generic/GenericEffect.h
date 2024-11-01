// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Data/SkillData.h>
#include <LWE_WOW/Class/Target.h>
#include <LWE_WOW/Class/Seeker.h>
#include <LWE_WOW/Class/Act.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericEffect.generated.h"

class UStaticMeshComponent;
class IInteractable;

// 스킬 이펙트 액터입니다.
UCLASS()
class LWE_WOW_API AGenericEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenericEffect();

	virtual void BeginPlay       ()                override;
	virtual void Tick            (float DeltaTime) override;
	virtual void PostActorCreated()                override;

public:
	using Skill = CAct::Skill;

public:
	CSeeker Seeker{ this }; // 전진 방향 계산 헬퍼

public:
	void Shoot(float DeltTime);
	void Enable(float DeltTime);

public:
	static void Spawn(Skill* InSkill, AGenericCharacter* InParent, AGenericCharacter* InTarget);

protected:
	UStaticMeshComponent* m_Mesh;             // 메시 컴포넌트
	AGenericCharacter*    m_Parent;           // 시전자입니다.
	AGenericCharacter*    m_Target;           // 타겟입니다.
	USkillData*           m_Data;             // 스킬 정보입니다.
	FSkillInfo            m_Info;             // 스킬 정보입니다
	float                 m_Interval;         // 스킬 시전 중 정보입니다.
	float                 m_Duration;         // 스킬 시전 중 정보입니다.
	int                   m_Level;            // 해당 스킬의 레벨입니다.
	bool                  m_bExecute = false; // 시전 플래그입니다.
};
