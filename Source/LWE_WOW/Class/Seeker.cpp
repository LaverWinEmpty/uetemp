#include "Seeker.h"

#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Generic/GenericCharacter.h>

CSeeker::CSeeker(AActor* InParent, float InErrorTolerance): m_Parent (InParent), m_ErrorTolerance(InErrorTolerance) {}

bool CSeeker::Setting(AGenericCharacter* In)
{
	if (!In->Target) {
		return false;
	}

	if (AGenericCharacter* Temp = In->Target.Get<AGenericCharacter>()) {
		Setting(In->GetActorLocation(), Temp->GetActorLocation());
		return true;
	}
	return false;
}

void CSeeker::Setting(AActor* InActor, const FVector& InEnd)
{
	Setting(InActor->GetActorLocation(), InEnd);
}

void CSeeker::Setting(const FVector& InCurrent, const FVector& InEnd)
{
	m_Start = InCurrent;
	m_End = InEnd;
}

bool CSeeker::SetTarget(AGenericCharacter* In)
{
	if (!In->Target) {
		return false;
	}

	if (AGenericCharacter* Temp = In->Target.Get<AGenericCharacter>()) {
		SetTarget(Temp->GetActorLocation());
		return true;
	}
	return false;
}

void CSeeker::SetTarget(const FVector& InTarget)
{
	m_End = InTarget;
}

void CSeeker::SetSpeed(float InSpeed)
{
	m_Speed = InSpeed;
}

float CSeeker::GetMovedSquared() const
{
	return (m_Parent->GetActorLocation() - m_Start).SquaredLength();
}

float CSeeker::GetTargetSquared() const
{
	return (m_End - m_Start).SquaredLength();
}

void CSeeker::View()
{
	// 객체를 따라가기 위해 회전
	FVector Direction = m_End - m_Parent->GetActorLocation();
	Direction.Normalize();
	m_Parent->SetActorRotation(Direction.Rotation());
}

void CSeeker::OnTick()
{
	FVector MoveTo = m_Parent->GetActorLocation() + (m_Parent->GetActorForwardVector() * m_Speed * CUtil::GetDeltaTime(m_Parent));
	m_Parent->SetActorLocation(MoveTo);
}

bool CSeeker::IsAtTarget()
{
	FVector Moved = m_End - m_Parent->GetActorLocation(), Prev = m_End - m_Start;
	if (FMath::IsNearlyZero(Moved.SizeSquared()))
		return true;
	return (Moved | Prev) <= 0; // dot
}