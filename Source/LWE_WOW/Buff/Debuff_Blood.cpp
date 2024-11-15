// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff_Blood.h"

#include <LWE_WOW/Generic/GenericCharacter.h>

void UDebuffBlood::OnBegin(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
}

void UDebuffBlood::OnTick(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
	InCaster->Damage(InPower, InTarget, false);
}

void UDebuffBlood::OnEnd(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const
{
}
