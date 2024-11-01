#include "Act.h"

#include <LWE_WOW/Data/SkillData.h>
#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>
#include <LWE_WOW/Manager/UIManager.h>


CAct::CAct(AGenericCharacter* InParent) : m_Parent(InParent)
{
}

CAct::~CAct()
{
}

bool CAct::Start(Skill* InSkill)
{
 	if (m_Current || !InSkill) return false;
	m_Target = nullptr; // 초기화

	ESkillFlag Flag = static_cast<ESkillFlag>(InSkill->Data->Option);
	// 타겟이 없는데 스킬 사용 시
	if (!m_Parent->Target) {
		// 자신에게 스킬 옵션이 켜져있지 않다면
		if ((Flag & ESkillFlag::IS_SELF) != ESkillFlag::NONE) {
			m_Target = m_Parent; // 자기 자신을 타겟
		}
	}
	else m_Target = Cast<AGenericCharacter>(m_Parent->Target.Get());

	// 타겟팅 실패
	if (!m_Target) {
		// 메세지 처리
		UUIManager::Instance(m_Parent)->SetMessageText(_T("대상이 없습니다."));
		return false;
	}

	if ((m_Target->GetActorLocation() - m_Parent->GetActorLocation()).SizeSquared() >=
		(InSkill->Info.Range * InSkill->Info.Range)) {
		UUIManager::Instance(m_Parent)->SetMessageText(_T("대상이 사거리를 벗어났습니다."));
		return false;
	}

	// 시야 체크
	if (InSkill->Data->Option & static_cast<uint8>(ESkillFlag::LIMIT_VIEW)) {
		FVector Direction = m_Target->GetActorLocation() - m_Parent->GetActorLocation();
		Direction.Normalize();
		float Dot = m_Parent->GetActorForwardVector() | Direction;

		if (Dot < 0) {
			UUIManager::Instance(m_Parent)->SetMessageText(_T("대상이 시야에 없습니다."));
			return false;
		}
	}

	// 타겟 적 아군 검사
	if ((InSkill->Data->Target & static_cast<uint8>(m_Parent->Target.GetRelation())) == 0) {
		UUIManager::Instance(m_Parent)->SetMessageText(_T("시전할 수 없는 대상입니다."));
		return false;
	}

	// 죽은 적한테 사용 불가능
	if (!(InSkill->Data->Option & static_cast<uint8>(ESkillFlag::IS_TO_DEAD))) {
		// 죽었는지 검사
		if (m_Target->IsDead) {
			UUIManager::Instance(m_Parent)->SetMessageText(_T("대상이 이미 죽었습니다."));
			return false;
		}
	}

	// 세팅
	m_Current  = InSkill;
	m_Charge = 0;

	return true;
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

		m_Parent->AnimationBase->Montage_Play(Anim);
		m_Parent->AnimationBase->Montage_JumpToSection("Attack_1", Anim); // 임시로 1번모션

		AGenericEffect::Spawn(m_Current, m_Parent, m_Target); // 실행 -> 스킬 이펙트 소환
		m_Current = nullptr;

		// UI 켜져있다면 끄기
		m_Parent->CastBar->Hide();
	}


	// 기타 처리
	else {
		// 캐스팅모션 실행
		UAnimMontage* Anim;
		if (m_Current->Data->CastingMotion) {
			Anim = m_Current->Data->CastingMotion; // 전용 모션 있다면 사용
		}
		else Anim = m_Parent->CastingMotion; // 아니면 기본 모션 사용
		m_Parent->AnimationBase->Montage_Play(Anim);

		// UI 출력
		// 현재 캐스팅 값 / 요구 캐스팅 값
		// 틱마다 호출
		m_Parent->CastBar->Show();
		m_Parent->CastBar->Update(m_Charge, m_Current->Info.Charge);
	}
}

