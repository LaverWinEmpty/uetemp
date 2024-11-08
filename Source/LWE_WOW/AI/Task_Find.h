// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LWE_WOW/AI/TaskBase.h"
#include "Task_Find.generated.h"

// 보스 대기 상태, 적을 탐색하는 단계 입니다.

UCLASS()
class LWE_WOW_API UTaskFind : public UTaskBase
{
	GENERATED_BODY()

public:
	UTaskFind();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent&, uint8*) override;
	virtual void                TickTask(UBehaviorTreeComponent&, uint8*, float InDelta) override;
};
