// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LWE_WOW/AI/TaskBase.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Task_Battle.generated.h"

class UAIAsyncTaskBlueprintProxy;

// 보스 전투 상태입니다.

UCLASS()
class LWE_WOW_API UTaskBattle : public UTaskBase
{
	GENERATED_BODY()

public:
	UTaskBattle();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent&, uint8*) override;
	virtual void                TickTask(UBehaviorTreeComponent&, uint8*, float InDelta) override;
};