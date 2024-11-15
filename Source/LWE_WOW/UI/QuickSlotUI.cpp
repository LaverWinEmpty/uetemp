// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotUI.h"

#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Generic/GenericInput.h>
#include <LWE_WOW/Data/SkillData.h>
#include <LWE_WOW/UI/SkillUI.h>

void UQuickSlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    // 모든 퀵슬롯은 동일한 아이콘 개수를 소유해야 합니다.
    check(!(SLOT_MAX && SLOT_MAX != m_Slots.Num()));
    SLOT_MAX = m_Slots.Num();

    m_Input = Cast<AGenericInput>(GetOwningPlayerPawn()->GetController());
}

FReply UQuickSlotUI::NativeOnMouseButtonDown(const FGeometry& In, const FPointerEvent& InEvent)
{
    return Super::NativeOnMouseButtonDown(In, InEvent);
}

FReply UQuickSlotUI::NativeOnMouseButtonUp(const FGeometry& In, const FPointerEvent& InEvent)
{ 
    // 선택된 게 없으면 무시합니다.
    if (!st_Selected) {
        return FReply::Unhandled();
    }

    // 실행
    UGenericSkill* Skill = Cast<UGenericSkill>(st_Selected->Info);
    if (Skill) {
        m_Input->PlayerAction(Skill);
    }
    return Super::NativeOnMouseButtonUp(In, InEvent);
}

void UQuickSlotUI::NativeOnDragDetected(const FGeometry& In,
                                        const FPointerEvent& InEvent,
                                        UDragDropOperation*& Out)
{
    Super::NativeOnDragDetected(In, InEvent, Out);
    st_Selected->Icon->SetVisibility(ESlateVisibility::Hidden);
}

bool UQuickSlotUI::NativeOnDrop(const FGeometry& In, const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
    // 자기 자신 또는 같은 슬롯이라면
    if (UQuickSlotUI* From = Cast<UQuickSlotUI>(st_SelectedParent)) {
        Swap(GetHoveredSlot(InEvent), st_Selected);
    }

    // 스킬창에서 스킬 가져온 거라면
    if (USkillUI* From = Cast<USkillUI>(st_SelectedParent)) {
        Slot* Temp = GetHoveredSlot(InEvent);
        Temp->Icon->SetBrush(st_Selected->Icon->GetBrush()); // 아이콘 복사 (변경)
        Temp->Info = st_Selected->Info;                      // 스킬 그냥 복사합니다.
    }

    st_Selected->Icon->SetVisibility(ESlateVisibility::Visible);
    return Super::NativeOnDrop(In, InEvent, InOperation);
}

void UQuickSlotUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    // 해당 칸에서 정보 삭제
    st_Selected->Icon->SetVisibility(ESlateVisibility::Visible);
    st_Selected->Icon->SetBrushFromTexture(EMPTY);
    st_Selected->Info = nullptr;

    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

void UQuickSlotUI::SetSlotInfo(EActionID InID, UGenericSkill* InSkill)
{
    ensure(GetUIIndex(InID) != WidgetIndex); // Indexing 오류

    int Index = GetSlotIndex(InID);

    m_Slots[Index].Info = InSkill;
    m_Slots[Index].Icon->SetBrushFromTexture(InSkill->Data->Image);

    check(m_Slots[Index].Info);
}

UGenericSkill* UQuickSlotUI::GetSlotInfo(EActionID In)
{
    // 해당 슬롯과 매칭된 ID인지 검사
    int Index = GetSlotIndex(In);
    if (Index < 0 || Index > SLOT_MAX) {
        check(false);
        return nullptr;
    }

    UGenericSkill* Skill = Get<UGenericSkill>(Index);
    return Skill;
}

EActionID UQuickSlotUI::GetSlotID(int InIndex)
{
    return static_cast<EActionID>(SLOT_MAX * WidgetIndex + IA_COMMAND_0 + InIndex);
}

int UQuickSlotUI::GetSlotIndex(EActionID InID)
{
    return (InID - IA_COMMAND_0) % SLOT_MAX;
}

UUIManager::EUIList UQuickSlotUI::GetUIIndex(EActionID InID)
{
    return static_cast<UUIManager::EUIList>(
        ((InID - IA_COMMAND_0) / SLOT_MAX) + UUIManager::UI_QUICKSLOT_0
    );
}