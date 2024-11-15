#include "SkillUI.h"

#include <LWE_WOW/Data/SkillData.h>
#include <LWE_WOW/Generic/GenericCharacter.h>

#include <LWE_WOW/Skill/Skill_Arrow.h>
#include <LWE_WOW/Skill/Skill_Fireball.h>
#include <LWE_WOW/Skill/Skill_Slash.h>
#include <LWE_WOW/Skill/Skill_Shuriken.h>

#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

USkillUI::USkillUI(const FObjectInitializer& In) : Super(In)
{
}

void USkillUI::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply USkillUI::NativeOnMouseButtonDown(const FGeometry& In, const FPointerEvent& InEvent)
{
	return Super::NativeOnMouseButtonDown(In, InEvent);
}

FReply USkillUI::NativeOnMouseButtonUp(const FGeometry& In, const FPointerEvent& InEvent)
{
	// 스킬레벨업 미구현
	// LearnSkill, RemoveSkill 여기서 사용

	return Super::NativeOnMouseButtonUp(In, InEvent);
}

void USkillUI::NativeOnDragDetected(const FGeometry& In, const FPointerEvent& InEvent, UDragDropOperation*& Out)
{
	return Super::NativeOnDragDetected(In, InEvent, Out);
}

bool USkillUI::NativeOnDrop(const FGeometry& In, const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(In, InEvent, InOperation);
}

void USkillUI::NativeOnDragCancelled(const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InEvent, InOperation);
}

void USkillUI::RegisterSkill()
{
	const TArray<USkillData*>& Skills = m_Parent->UseableSkillList;

	// 실행은 되나 일단은 에러
	int Loop;
	if (m_Slots.Num() != Skills.Num()) {
		ensure(false);
		Loop = m_Slots.Num() < Skills.Num() ? m_Slots.Num() : Skills.Num();
	}
	else Loop = m_Slots.Num();

	// 임시
	for (int i = 0; i < Loop; ++i) {
		UGenericSkill* NewSkill = NewObject<UGenericSkill>(this);
		NewSkill->SetData(Skills[i], 1); // 1레벨 기준으로 세팅
		m_Slots[i].Icon->SetBrushFromTexture(Skills[i]->Image);
		m_Slots[i].Info = NewSkill;

		// 임시: 그냥 다 배운 거로 초기화 해둡니다.
		m_Parent->LearnSkill(NewSkill);
	}
}
