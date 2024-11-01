#include "GenericPlayer.h"

#include <LWE_WOW/Generic/GenericIcon.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/Data/SkillData.h>

auto AGenericPlayer::GetType() const -> ETargetType
{
	return ETargetType::PLAYER;
}

void AGenericPlayer::BeginPlay()
{
	Super::BeginPlay();

	Team = ETeamType::A; // 임시

	// 테스트 코드: 스킬 바인딩
	for (int i = 0; i < SkillList.Num(); ++i) {
		SetSkillSlot(static_cast<EActionID>(IA_COMMAND_0 + i), &SkillList[SkillTable[i]]);
	}
}

void AGenericPlayer::SetSkillSlot(EActionID InID, Skill* InSkill)
{
	// 스킬 정보가 없거나 빈 데이터면
	if (!InSkill || !SkillList.Find(InSkill->Data)) {
		return;
	}

	// 슬롯에 세팅
	if (SkillSlots.Find(InID)) {
		SkillSlots[InID] = &SkillList[InSkill->Data];
	}
	else SkillSlots.Add(InID, &SkillList[InSkill->Data]);

	// 임시
	// 위젯 가져와서 이미지 세팅
	auto SlotWidget = UUIManager::Instance(this)->Widgets[UUIManager::EUIList::UI_SKILL_SLOT_1];
	auto Slots = Cast<UGenericIcon>(SlotWidget);
	Slots->Slot[InID - IA_COMMAND_0]->SetBrushFromTexture(InSkill->Data->Image);
}