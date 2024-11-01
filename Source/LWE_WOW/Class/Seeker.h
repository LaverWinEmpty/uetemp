#pragma once

#include "CoreMinimal.h"

class AActor;
class AGenericCharacter;

/*
	전진 자동 이동 시 계산을 도울 헬퍼 클래스입니다.
	캐릭터는 사용하지 않음 <- 리펙터링?
*/

class CSeeker {
public:
	CSeeker(AActor* InParent, float InErrorTolerance = UE_SMALL_NUMBER);

public:
	bool Setting(AGenericCharacter* InInteractableActor);
	void Setting(AActor* InActor, const FVector& InEnd);
	void Setting(const FVector& InCurrent, const FVector& InEnd);

public:
	bool SetTarget(AGenericCharacter* InInteractableActor);
	void SetTarget(const FVector& InTarget);

public:
	void SetSpeed(float InSpeed);

public:
	float GetMovedSquared()  const;
	float GetTargetSquared() const;

public:
	void View();
	void OnTick(); // MovementComponent 없는 경우 사용

public:
	bool IsAtTarget();

public:
	const FVector& Start = m_Start;
	const FVector& End   = m_End;

private:
	AActor* m_Parent;
	FVector m_Start;
	FVector m_End;
	float   m_Speed;
	float   m_ErrorTolerance;
};