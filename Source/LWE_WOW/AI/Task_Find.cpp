// Fill out your copyright notice in the Description page of Project Settings.

#include "Task_Find.h"

#include "Blueprint/AIAsyncTaskBlueprintProxy.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include <LWE_WOW/Generic/GenericBossAI.h>
#include <LWE_WOW/Character/Boss.h>

UTaskFind::UTaskFind()
{
	NodeName = "WaitTask";
	bCreateNodeInstance = true;
	bTickIntervals = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UTaskFind::ExecuteTask(UBehaviorTreeComponent& InBTComponent, uint8* InNodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UTaskFind::TickTask(UBehaviorTreeComponent& InBTComponent, uint8* InNodeMemory, float InDelta)
{
	Super::TickTask(InBTComponent, InNodeMemory, InDelta);


	TArray<AActor*> Overlapped;
	m_Owner->BossArea->GetOverlappingActors(Overlapped);
	for (AActor* Actor : Overlapped) {
		if (AGenericCharacter* Character = Cast<AGenericCharacter>(Actor)) {
			if (Character->IsDead) {
				continue; // 죽었으면 무시
			}
			if (Character->IsIgnore) {
				continue; // 타겟팅할 수 없음
			}
			// 타겟 셋팅
			if (m_Owner->GetRelation(Character) != ERelationType::HARM) {
				continue; // 적이 아님
			}
			
			// 타겟을 지정합니다.
			m_Owner->Target.Setting(Character);
			FinishLatentTask(InBTComponent, EBTNodeResult::Succeeded);
			break;
		}
	}
}
