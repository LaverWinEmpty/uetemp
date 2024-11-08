// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Common/Constants.h>

#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "LWE_WOW/Generic/GenericCharacter.h"
#include "Mob.generated.h"

class USphereComponent;

/*  자동으로 싸우는 객체입니다
 *  간단하게 이동 / 공격 정도만 구현되어 있어 AI컨트롤러를 사용하지 않습니다.
 */
UCLASS()
class LWE_WOW_API AMob : public AGenericCharacter
{
	GENERATED_BODY()

public:
	AMob();

	virtual void BeginPlay() override;
	virtual void Tick(float) override;
	virtual void MoveTick() override;
	virtual void OnHit(AGenericCharacter* InHiter) override;
	
	void StartBattle();
	void BattleTick();
	void Tracking();

	UGenericSkill* SelectSkill();

protected:
	float m_MoveUpdateTime   = 0; // 랜덤
	float m_BattleUpdateTime = 1;
	float m_Time             = 0;
	float m_Distance         = 0; // 추적 간격, 스킬 사거리에 따라 달라집니다.

public:
	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveValue = 0.05;

	// 이동 범위입니다.
	UPROPERTY(EditAnywhere, Category = "AI")
	float WaitAreaRange = 300;

	// 적을 따라가는 범위입니다.
	UPROPERTY(EditAnywhere, Category = "AI")
	float FollowAreaRange = 3000;

	// 적 발견 거리입니다. (반지름)
	UPROPERTY(EditAnywhere, Category = "AI")
	float FindDistance = 1000;

	// 스킬 개수 == 패턴 개수
	int SkillCount;

	static constexpr float UPDATE_TIME_MIN = 1; // 이동 주기입니다
	static constexpr float UPDATE_TIME_MAX = 5; // 이동 주기입니다.

	FVector3d InitialLocation;
	FVector3d Moved = { 0, 0, 0 };

public:
	USphereComponent* Finder;

protected:
	UCharacterMovementComponent* m_Movement;

protected:
	UGenericSkill* SelectedSkill;

protected:
	bool IsBattle   : 1 = false;
	bool IsTracking : 1 = false;
	bool IsReturn   : 1 = false;
};
