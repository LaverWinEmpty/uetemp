#include "GenericPlayer.h"

#include <LWE_WOW/Generic/GenericInput.h>
#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/Data/SkillData.h>
#include <LWE_WOW/UI/QuickSlotUI.h>
#include <LWE_WOW/Data/ItemData.h>
#include <LWE_WOW/UI/SkillUI.h>

auto AGenericPlayer::GetType() const -> ETargetType
{
	return ETargetType::PLAYER;
}

void AGenericPlayer::Dead()
{
	Super::Dead();
	UUIManager::Instance(this)->OnDead();
}

void AGenericPlayer::BeginPlay()
{
	Super::BeginPlay();

	Team = ETeamType::A; // 임시
}

void AGenericPlayer::View(const FVector& InTarget)
{
	AGenericInput* PlayerHnadler = Cast<AGenericInput>(GetController());

	// 카메라의 위치를 유지하기 위해 저장합니다.
	FRotator CameraRotator = PlayerHnadler->GetCameraArm()->GetComponentRotation();

	Super::View(InTarget);

	// 카메라 각도를 유지합니다.
	PlayerHnadler->GetCameraArm()->SetWorldRotation(CameraRotator);
}

void AGenericPlayer::Initialize()
{
	Super::Initialize();
	Cast<USkillUI>(UUIManager::Instance(this)->Widgets[UUIManager::UI_SKILL])->RegisterSkill();
}

void AGenericPlayer::SetSkillSlot(EActionID InID, UGenericSkill* InSkill)
{
	// 스킬 정보가 없거나 빈 데이터면
	if (!InSkill || !SkillList.Find(InSkill->Data)) {
		return;
	}

	//// 슬롯에 세팅
	//if (SkillSlots.Find(InID)) {
	//	SkillSlots[InID] = SkillList[InSkill->Data];
	//}
	//else SkillSlots.Add(InID, SkillList[InSkill->Data]);

	// 위젯 가져옴
	UQuickSlotUI* Slots = Cast<UQuickSlotUI>(
		UUIManager::Instance(this)->Widgets[UQuickSlotUI::GetUIIndex(InID)]
	);

	// ID를 인덱스로 변환 후 해당 위치에 저장
	Slots->SetSlotInfo(InID, InSkill);
}