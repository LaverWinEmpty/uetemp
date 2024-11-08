// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Battle.h"

#include <LWE_WOW/Character/Boss.h>

UTaskBattle::UTaskBattle()
{
	NodeName = "BattleTask";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UTaskBattle::ExecuteTask(UBehaviorTreeComponent& InBTComponent, uint8* InNodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UTaskBattle::TickTask(UBehaviorTreeComponent& InBTComponent, uint8*, float InDelta)
{
	AGenericCharacter* Target = m_Owner->Target.Get<AGenericCharacter>();
	if (Target == nullptr) {
		ensure(false);
		FinishLatentTask(InBTComponent, EBTNodeResult::Failed);
	}

	// 죽은 상태 등 무시당하는 상태인지 검사합니다.
	if (Target->IsDead || Target->IsIgnore) {
		// 없다면 다시 타겟을 고릅니다
		FinishLatentTask(InBTComponent, EBTNodeResult::Failed);
		return;
	}

	// 만약 적이 사거리 밖에 있다면
	if (!CUtil::IsIn(m_Owner, Target, m_Owner->FindDistance)) {
		// 적이 영역 밖으로 도망갔는지 검사합니다.
		if (!m_Owner->BossArea->IsOverlappingActor(Target)) {
			// 새로운 적을 찾습니다
			FinishLatentTask(InBTComponent, EBTNodeResult::Failed);
			return;
		}
		// 스킬 끝나고 쫓아갑니다.
		if (!m_Owner->AnimationBase->IsAnyMontagePlaying()) {
			FinishLatentTask(InBTComponent, EBTNodeResult::Succeeded);
		}
	}

	else {
		// 랜덤 스킬 (패턴) 가져옵니다
		UGenericSkill* Pattern = m_Owner->SelectSkill();
		// 사용가능한 패턴이 있다면 사용합니다.
		if (Pattern) {
			m_Owner->View(Target->GetActorLocation());
			m_Owner->Act.Start(Pattern);
		}
	}
}


//
//
//	if (!m_Owner->Act) {
//		FinishLatentTask(InOwnerBTComponent, EBTNodeResult::Succeeded);
//	}
//}