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
class USphereComponent;
class UGenericSkill;
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
	CSeeker Seeker{ this }; // 전진 방향 계산 헬퍼

public:
	void Shoot(float DeltTime);
	void Enable(float DeltTime);

public:
	using FnSkillMethod = void (USkillData::*)(UGenericSkill*, AGenericCharacter*, AGenericCharacter*, AGenericEffect*) const;

	// Execute, OnTick, Final 호출하는 함수입니다.
	// 내부적으로 non-target 스킬인지에 따라 해당 함수를 호출하게 되어있습니다.
	void SkillMethod(FnSkillMethod);

public:
	static AGenericEffect* Spawn(UGenericSkill* InSkill, AGenericCharacter* InParent, AGenericCharacter* InTarget);
	static AGenericEffect* Spawn(UGenericSkill* InSkill, AGenericCharacter* InParent, AGenericCharacter* InTarget, const FVector& InPos);

	// 스킬 데이터를 기반으로 내부에서 생성할 때 사용합니다.
	static AGenericEffect* Spawn(USkillData* InSkillData, int InLevel, const TCHAR* const InName,
		AGenericCharacter* InParent, AGenericCharacter* InTarget, const FVector& InPos);

private:
	static AGenericEffect* Spawn(USkillData*, AGenericCharacter*, const FVector&);
	void                   Initialize(USkillData*, int, const TCHAR* const, AGenericCharacter*, AGenericCharacter*, const FVector&);

protected:
	UPROPERTY()
	UGenericSkill* m_Status; // Strong Reference: 스킬 상태 == 정보입니다.

protected:
	USphereComponent*     m_Area;             // 캡슐(범위) 컴포넌트
	UStaticMeshComponent* m_Mesh;             // 메시 컴포넌트
	AGenericCharacter*    m_Parent;           // 시전자입니다.
	AGenericCharacter*    m_Target;           // 타겟입니다.
	float                 m_Interval;         // 스킬 시전 중 정보입니다.
	float                 m_Duration;         // 스킬 시전 중 정보입니다.
	int                   m_Level;            // 해당 스킬의 레벨입니다.
	bool                  m_bExecute = false; // 시전 플래그입니다.
};
