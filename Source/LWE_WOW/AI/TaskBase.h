// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TaskBase.generated.h"

class ABoss;
class AGenericBossAI;

/**
 * 
 */
UCLASS(Abstract)
class LWE_WOW_API UTaskBase : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual void SetOwner(AActor*) override;

protected:
	ABoss*          m_Owner;
	AGenericBossAI* m_Handler;
};

