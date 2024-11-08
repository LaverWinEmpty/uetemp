// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericBossAI.h"

#include <BehaviorTree/BehaviorTree.h>

AGenericBossAI::AGenericBossAI()
{
}

void AGenericBossAI::OnPossess(APawn* In)
{
	Super::OnPossess(In);

	Initialize();
}

void AGenericBossAI::Initialize()
{
	auto BT = LoadObject<UBehaviorTree>(nullptr, _T("/Game/AI/BPBT_Boss.BPBT_Boss"));
	ensure(BT);
	RunBehaviorTree(BT);
}
