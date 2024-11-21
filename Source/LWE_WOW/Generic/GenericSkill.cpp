// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericSkill.h"

#include <LWE_WOW/Data/SkillData.h>

void UGenericSkill::SetData(USkillData* InData, int InLevel)
{
	Data = InData;
	Icon = InData->Image;
	SetLevel(InLevel);
}

void UGenericSkill::SetLevel(int InLevel)
{
	if (!Data) {
		return;
	}
	// 알아서 계산
	Level = InLevel;
	Info = Data->Calculate(Level);
}
