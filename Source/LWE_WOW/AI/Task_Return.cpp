// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Return.h"
#include "Navigation/PathFollowingComponent.h"

#include <LWE_WOW/Character/Boss.h>
#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Generic/GenericBossAI.h>

UTaskReturn::UTaskReturn()
{
	NodeName = "ReturnTask";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UTaskReturn::ExecuteTask(UBehaviorTreeComponent& InOwnerBTComponent, uint8* InNodeMemory)
{
	m_Owner->Act.Cancel();
	m_Handler->MoveTo(m_Owner->InitialLocation);

	return EBTNodeResult::InProgress;
}

void UTaskReturn::TickTask(UBehaviorTreeComponent& InOwnerBTComponent, uint8*, float InDelta)
{
	EPathFollowingStatus::Type Status =
		m_Handler->GetPathFollowingComponent()->GetStatus();

	// 정지했다면
	if (Status == EPathFollowingStatus::Idle) {
		m_Handler->StopMovement();
		// 대기 상태로
		FinishLatentTask(InOwnerBTComponent, EBTNodeResult::Succeeded);
	}
}
