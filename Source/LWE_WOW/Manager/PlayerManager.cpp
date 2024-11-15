// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "Kismet/GameplayStatics.h"



UPlayerManager* UPlayerManager::Instance(UObject* InWorldContextObject)
{
    if (void* Ptr = UGameplayStatics::GetGameInstance(InWorldContextObject->GetWorld())) {
        return static_cast<UGameInstance*>(Ptr)->GetSubsystem<UPlayerManager>();
    }
    return nullptr;
}

void UPlayerManager::SaveOnMoveLevel(AGenericPlayer* In)
{
    UUIManager* UI = UUIManager::Instance(this);

    UInvenUI*     Inven  = Cast<UInvenUI>    (UI->Widgets[UUIManager::UI_INVENTORY  ]);
    UEquipUI*     Equip  = Cast<UEquipUI>    (UI->Widgets[UUIManager::UI_EQUIPMENT  ]);
    USkillUI*     Skill  = Cast<USkillUI>    (UI->Widgets[UUIManager::UI_SKILL      ]);
    UQuickSlotUI* Quick0 = Cast<UQuickSlotUI>(UI->Widgets[UUIManager::UI_QUICKSLOT_0]);
    UQuickSlotUI* Quick1 = Cast<UQuickSlotUI>(UI->Widgets[UUIManager::UI_QUICKSLOT_1]);


    for (int i = 0; i < ; ++i) {
        Slots.Add(TStrongObjectPtr<UObject>(Inven->m_Slots[i].Info));
    }
    for (int i = 0; i < 1; ++i) {
        Slots.Add(TStrongObjectPtr<UObject>(Equip->m_Slots[i].Info));
    }
    for (int i = 0; i < 4; ++i) {
        SkillList[i] = TStrongObjectPtr<UObject>(Skill->m_Slots[i].Info);
    }
    for (int i = 0; i < 12; ++i) {
        QuickSlots[i] = TStrongObjectPtr<UObject>(Quick0->m_Slots[i].Info);
    }
    for (int i = 0; i < 12; ++i) {
        QuickSlots[i + 12] = TStrongObjectPtr<UObject>(Quick1->m_Slots[i].Info);
    }
}

void UPlayerManager::LoadOnMoveLevel(AGenericPlayer*& Out)
{

}
