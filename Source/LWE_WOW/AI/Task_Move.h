// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LWE_WOW/AI/TaskBase.h"
#include "Task_Move.generated.h"

// 캐릭터 추적 상태입니다.
UCLASS()
class LWE_WOW_API UTaskMove : public UTaskBase
{
	GENERATED_BODY()

public:
	UTaskMove();

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent&, uint8*);
	void                TickTask(UBehaviorTreeComponent&, uint8*, float);
};	