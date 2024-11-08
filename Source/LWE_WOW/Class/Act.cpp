#include "Act.h"

#include <LWE_WOW/Data/SkillData.h>
#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>
#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/UI/GageUI.h>


bool CAct::IsOK(EResult Result)
{
	return Result == ACT_SUCCEED || Result == ACT_ACTING;
}

CAct::CAct(AGenericCharacter* InParent) : m_Parent(InParent)
{
}

CAct::~CAct()
{
}

void CAct::Cancel()
{
	if (m_Current == nullptr || m_Charge >= m_Current->Info.Charge) {
		// 이미 시전 완료 됨
		return;
	}

	m_Current = nullptr;
	m_Target  = nullptr;
	m_Charge  = 0;
	// UI 끄기
	m_Parent->CastBar->Hide();

	// 캐스팅 모션 종료
	m_Parent->StopAnimMontage(m_Parent->CastingMotion);
}

auto CAct::Start(UGenericSkill* InSkill)->EResult
{
	if (m_Current || Notify || !InSkill)
		return ACT_ACTING;

	m_Target = nullptr; // 초기화

	ESkillFlag Flag = static_cast<ESkillFlag>(InSkill->Data->Option);
	// 타겟이 없는데 스킬 사용 시
	if (!m_Parent->Target) {
		// 자신에게 스킬 옵션이 켜져있지 않다면
		if ((Flag & ESkillFlag::IS_CAN_SELF) != ESkillFlag::NONE) {
			m_Target = m_Parent; // 자기 자신을 타겟
		}
	}
	else m_Target = Cast<AGenericCharacter>(m_Parent->Target.Get());


	// 쿨타임
	if (InSkill->Cooldown != 0) {
		return ACT_COOLDOWN;
	}

	// 논타겟 스킬이면 실행, 아닌 경우 검사
	if (!(InSkill->Data->Option & static_cast<uint8>(ESkillFlag::IS_NON_TARGET))) {
		// 타겟팅 실패
		if (!m_Target) {
			return ACT_TARGET_NOT_EXISTS;
		}

		// 사거리 초과
		if ((m_Target->GetActorLocation() - m_Parent->GetActorLocation()).SizeSquared() >=
			(InSkill->Info.Range * InSkill->Info.Range)) {
			return ACT_OUT_OF_RANGE;
		}

		// 시야 체크
		if (InSkill->Data->Option & static_cast<uint8>(ESkillFlag::LIMIT_VIEW)) {
			FVector Direction = m_Target->GetActorLocation() - m_Parent->GetActorLocation();
			Direction.Normalize();
			float Dot = m_Parent->GetActorForwardVector() | Direction;

			if (Dot < 0) {
				return ACT_OUT_OF_VIEW;
			}
		}

		// 타겟 유효성 검사
		if ((InSkill->Data->Target & static_cast<uint8>(m_Parent->Target.GetRelation())) == 0) {
			return ACT_TARGET_IS_INVALID;
		}

		// 죽은 적한테 사용 불가능
		if (!(InSkill->Data->Option & static_cast<uint8>(ESkillFlag::IS_CAN_DEAD))) {
			// 죽었는지 검사
			if (m_Target->IsDead) {
				return ACT_TARGET_IS_DIED;
			}
		}
	}

	// 세팅
	m_Current = InSkill;
	m_Charge  = 0;

	// 캐스팅 있음
	if (InSkill->Info.Charge != 0) {
		// 이동 중 사용 불가능
		if (m_Parent->IsMoving() || m_Parent->IsJumping) {
			m_Current = nullptr; // 다시 초기화
			return ACT_CAN_NOT_MOVING;
		}

		ChargeMotionPlay();
		m_Parent->CastBar->Show(); // UI
	}

	return ACT_SUCCEED;
}

void CAct::OnTick(float DeltaTime)
{
	if (!m_Current) { return; }

	// 차지
	// 유형에 따라 누르고 있을 때, 혹은 상관 없이 구분해야 하나 임시로 처리
	m_Charge += DeltaTime;

	// 즉발 가능성 있어서 먼저 처리
	if (m_Charge >= m_Current->Info.Charge) {
		// 공격 모션 실행
		UAnimMontage* Anim;
		if (m_Current->Data->ExecuteMotion) {
			Anim = m_Current->Data->ExecuteMotion; // 전용 모션 있다면 사용
		}
		else Anim = m_Parent->AttackMotion; // 아니면 기본 모션 사용

		// 시전 시 
		if (m_Target) {
			m_Parent->View(m_Target->GetActorLocation());
		}

		m_Parent->AnimationBase->Montage_Play(Anim);
		if (!m_Current->Data->ExecuteMotion) {
			m_Parent->AnimationBase->Montage_JumpToSection("Attack_1", Anim); // 임시로 1번모션
		}

		// 현재 스킬이 null이 되어야 작동하기 때문에
		// 노티파이에 쓸 포인터로 이동 후 nullptr 세팅
		Notify = m_Current;
		m_Current = nullptr;
	}


	// 기타 처리
	else {
		// 시전 중 죽어버렸으면
		if (m_Target && m_Target->IsDead) {
			Cancel(); // 취소
		}

		else {
			// UI 출력
			// 현재 캐스팅 값 / 요구 캐스팅 값
			// 틱마다 호출
			m_Parent->CastBar->Update(m_Charge, m_Current->Info.Charge);
		}
	}
}

void CAct::ChargeMotionPlay()
{
	// 캐스팅모션 실행
	UAnimMontage* Anim;
	if (m_Current->Data->CastingMotion) {
		Anim = m_Current->Data->CastingMotion; // 전용 모션 있다면 사용		
	}
	else Anim = m_Parent->CastingMotion; // 아니면 기본 모션 사용
	m_Parent->AnimationBase->Montage_Play(Anim);
}

void CAct::OnNotify()
{
	if (Notify) {
		AGenericEffect::Spawn(Notify, m_Parent, m_Target); // 실행 -> 스킬 이펙트 소환

		// UI 켜져있다면 끄기
		m_Parent->CastBar->Hide();
		Notify->Cooldown = Notify->Info.Cooldown; // 쿨타임 적용 후
		Notify = nullptr;                         // 해제
	}
}