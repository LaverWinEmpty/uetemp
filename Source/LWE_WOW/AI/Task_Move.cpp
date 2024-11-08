// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Move.h"

#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Character/Boss.h>
#include <LWE_WOW/Generic/GenericBossAI.h>

UTaskMove::UTaskMove()
{
	NodeName = "MoveTask";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}


EBTNodeResult::Type UTaskMove::ExecuteTask(UBehaviorTreeComponent& InBTComponent, uint8* InNodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UTaskMove::TickTask(UBehaviorTreeComponent& InBTComponent, uint8*, float InDelta)
{
	AActor* Target = m_Owner->Target.Get<AActor>();

	m_Handler->MoveToActor(Target);
	m_Owner->View(Target->GetActorLocation());

	// 공격 범위 안으로 들어 왔다면
	if (CUtil::IsIn(m_Owner, Target, m_Owner->FindDistance)) {
		// 정지하고
		m_Handler->StopMovement();
		// 전투 태스크로 진입합니다.
		FinishLatentTask(InBTComponent, EBTNodeResult::Succeeded);
	}
}
