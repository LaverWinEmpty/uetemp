// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffData.h"

#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Generic/GenericCharacter.h>

int UBuffData::AdjustLevel(int InLevel) const
{
	if      (InLevel > MaxLevel) InLevel = MaxLevel;
	else if (InLevel < 1)        InLevel = 1;
	return InLevel;
}

FBuffInfo::State UBuffData::Initialize() const
{
	FBuffInfo::State Temp;
	Temp.Duration   = 0;
	Temp.Interval   = 0;
	Temp.Overlapped = 1;
	return Temp;
}

FBuffInfo UBuffData::Calculate(int InLevel) const
{
	FBuffInfo Info;
	Info.Power      = CUtil::CoefficientFormula     (Default.Power,      Coefficient.Power,      InLevel);
    Info.Duration   = CUtil::CoefficientFormula     (Default.Duration,   Coefficient.Duration,   InLevel);
    Info.Interval   = CUtil::CoefficientFormula     (Default.Interval,   Coefficient.Interval,   InLevel);
    Info.Overlapped = CUtil::CoefficientFormula<int>(Default.Overlapped, Coefficient.Overlapped, InLevel);
	return Info;
}

void UBuffData::OnBegin(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const { /* Pure */ }

void UBuffData::OnTick(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const { /* Pure */ }

void UBuffData::OnEnd(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const { /* Pure */ }
