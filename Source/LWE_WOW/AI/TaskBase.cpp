// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskBase.h"

#include <LWE_WOW/Character/Boss.h>
#include <LWE_WOW/Generic/GenericBossAI.h>

void UTaskBase::SetOwner(AActor* InAIController)
{
	m_Handler = Cast<AGenericBossAI>(InAIController);
	check(m_Handler);
	m_Owner = Cast<ABoss>(m_Handler->GetPawn());
	check(m_Owner);
}