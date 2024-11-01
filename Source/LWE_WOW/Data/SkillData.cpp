// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillData.h"

#include <LWE_WOW/Generic/GenericCharacter.h>

int USkillData::AdjustLevel(int InLevel) const
{
	if      (InLevel > MaxLevel)   InLevel = MaxLevel;
	else if (InLevel < LearnLevel) InLevel = LearnLevel;
	return InLevel;
}

FSkillInfo USkillData::Calculate(int InLevel) const
{
	InLevel = AdjustLevel(InLevel);

	FSkillInfo Info;

	Info.Power    = CUtil::CoefficientFormula     (Default.Power,    Coefficient.Power,    InLevel);
	Info.Energe   = CUtil::CoefficientFormula     (Default.Energe,   Coefficient.Energe,   InLevel);
	Info.Area     = CUtil::CoefficientFormula     (Default.Area,     Coefficient.Area,     InLevel);
	Info.Range    = CUtil::CoefficientFormula     (Default.Range,    Coefficient.Range,    InLevel);
	Info.Cooldown = CUtil::CoefficientFormula     (Default.Cooldown, Coefficient.Cooldown, InLevel);
	Info.Charge   = CUtil::CoefficientFormula     (Default.Charge,   Coefficient.Charge,   InLevel);
	Info.Duration = CUtil::CoefficientFormula     (Default.Duration, Coefficient.Duration, InLevel);
	Info.Interval = CUtil::CoefficientFormula     (Default.Interval, Coefficient.Interval, InLevel);
	Info.Speed    = CUtil::CoefficientFormula     (Default.Speed,    Coefficient.Speed,    InLevel);
	Info.Scale    = CUtil::CoefficientFormula     (Default.Scale,    Coefficient.Scale,   InLevel);
	Info.Target   = CUtil::CoefficientFormula<int>(Default.Target,   Coefficient.Target,   InLevel);

	return Info;
}

USkillData::USkillData()
{
	Default.Scale = 1;
}

void USkillData::Execute(const FSkillInfo&, AGenericCharacter*, AGenericCharacter*) const { /* Pure */ }
void USkillData::Final  (const FSkillInfo&, AGenericCharacter*, AGenericCharacter*) const { /* Pure */ }
void USkillData::OnTick (const FSkillInfo&, AGenericCharacter*, AGenericCharacter*) const { /* Pure */ }

void USkillData::ApplyBuff(AGenericCharacter* InTarget) const
{
	// 효과 전달
	for (int i = 0; i < Apply.Num(); ++i) {
		InTarget->Effects.Add(Apply[i]->GetDefaultObject<UBuffData>(), 0);
	}
}