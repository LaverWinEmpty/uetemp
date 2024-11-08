// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LWE_WOW/AI/TaskBase.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Task_Return.generated.h"

// 보스 복귀 상태입니다.

UCLASS()
class LWE_WOW_API UTaskReturn : public UTaskBase
{
	GENERATED_BODY()

public:
	UTaskReturn();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent&, uint8*) override;
	virtual void                TickTask(UBehaviorTreeComponent&, uint8*, float InDelta) override;
};
