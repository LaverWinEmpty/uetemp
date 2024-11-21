// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "Kismet/GameplayStatics.h"

#include <LWE_WOW/UI/EquipUI.h>
#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericItem.h>
#include <LWE_WOW/Generic/GenericPlayer.h>

UPlayerManager* UPlayerManager::Instance(UObject* InWorldContextObject)
{
    if (UWorld* World = InWorldContextObject->GetWorld()) {
        if (UGameInstance* GI = UGameplayStatics::GetGameInstance(World)) {
            if (UPlayerManager* Self = GI->GetSubsystem<UPlayerManager>()) {
                return Self;
            }
        }
    }
    return nullptr;
}

void UPlayerManager::Save(AGenericCharacter* In)
{
    // Lobby 캐릭터 등 방지
    if (!Cast<AGenericPlayer>(In)) {
        return;
    }

    // 저장 시점에 슬롯 데이터를 가져와 저장
    SetUISlotInfo();

    int Index = 0;
    int Loop = Slots.Num();

    // UI nullptr check
    for (auto Itr = UI.begin(); Itr != UI.end(); ++Itr) {
        if (*Itr == nullptr) {
            IsSaved = false;
            return;
        }
    }

    int Cnt = UI[0]->GetSlotCount();
    for (int i = 0, j = 0; i < Loop; ++i) {

        // 저장할 슬롯 위치 i = 실제 슬롯 위치 j
        Slots[i] = UI[Index]->GetSlot(j)->Info; // 강참조

        if (++j == Cnt) {
            if (++Index < UI.Num()) {
                Cnt = UI[Index]->GetSlotCount();
                j = 0;
            } // end if
        } // end if
    } // end for

    // 임시로 장비 다 벗김
    UEquipUI* Equip = Cast<UEquipUI>(UUIManager::Instance(this)->Widgets[UUIManager::UI_EQUIPMENT]);
    for (int i = 0; i < Equip->GetSlotCount(); ++i) {
        if (UGenericItem* Item = Cast<UGenericItem>(Equip->GetSlot(i)->Info)) {
            Item->OnUnequipment(In);
        }
    }

    // 임시로 체력만
    HP     = In->HP.Value;
    Status = In->Source;

    IsSaved = true;
}

void UPlayerManager::Load(AGenericCharacter* Out)
{
    if (!Out) {
        check(false);
        return;
    }

    // 불러오는 시점의 슬롯에 저장된 데이터 이동
    SetUISlotInfo();

    // 슬롯 사이즈만큼 루프
    int Index = 0;
    int Loop = Slots.Num();

    // UI nullptr check
    for (auto Itr = UI.begin(); Itr != UI.end(); ++Itr) {
        if (*Itr == nullptr) {
            check(false);
            return;
        }
    }

    int Cnt = UI[0]->GetSlotCount();
    for (int i = 0, j = 0; i < Loop; ++i) {
        
        // 실제 슬롯 위치 j = 저장할 슬롯 위치 i
        UI[Index]->SetSlot(Slots[i], j);
        Slots[i] = nullptr; // 이동 처리

        if (++j == Cnt) {
            if (++Index < UI.Num()) {
                Cnt = UI[Index]->GetSlotCount();
                j   = 0;
            } // end if
        } // end if
    } // end for

    Out->HP.Value = HP;
    Out->Source   = Status;

    // 원래는 로딩된 데이터를 사용해야 하나
    // Pawn 자체가 매번 생성되므로 임시로 재생성
    UEquipUI* Equip = Cast<UEquipUI>(UI[0]); // 임시로 하드코딩
    for (int i = 0; i < Equip->GetSlotCount(); ++i) {
        if (UGenericItem* Slot = Cast<UGenericItem>(Equip->GetSlot(i)->Info)) {
            Slot->OnEquipment(Out);
        }
    } // end for

    // 매번 스킬 로딩하면서 새 스킬이 되어서
    // 해시 기반이라 그냥 순회하면서 다시 가져왔으나
    // Leak 발생하여 (게임 끄기 전까지) 1회만 로딩되게 수정
    // 혹시 몰라 코드 주석 처리
    //for (int UIIndex = 1; UIIndex <= 2; ++UIIndex) {
    //    // 퀵슬롯 전체 순회
    //    for (int SlotIndex = 0; SlotIndex < UI[UIIndex]->GetSlotCount(); ++SlotIndex) {
    //        // nullptr이 아니면
    //        if (UGenericSkill* Skill = UI[UIIndex]->Get<UGenericSkill>(SlotIndex)) {
    //            // 해시 기반으로 찾아옴. 주소만 바꿈
    //            UI[UIIndex]->GetSlot(SlotIndex)->Info = Out->SkillList[Skill->Data];
    //        }
    //    }
    //}
}

void UPlayerManager::SetUISlotInfo()
{
    UUIManager* UIM = UUIManager::Instance(this);

    // 주소 자체는 매번 달라지나 개수는 고정
    USlotUI* List[] = {
        Cast<USlotUI>(UIM->Widgets[UUIManager::UI_EQUIPMENT]),   // 현재 장비창 정보
        Cast<USlotUI>(UIM->Widgets[UUIManager::UI_QUICKSLOT_0]), // 현재 퀵슬롯 정보 1
        Cast<USlotUI>(UIM->Widgets[UUIManager::UI_QUICKSLOT_1]), //                  2
        Cast<USlotUI>(UIM->Widgets[UUIManager::UI_INVENTORY]),   // 현재 인벤토리 정보
        Cast<USlotUI>(UIM->Widgets[UUIManager::UI_SKILL]),       // 현재 스킬 정보
    };
    
    int Loop = sizeof(List) / sizeof(*List);
    if (!UI.Num()) {
        UI.Init(nullptr, Loop);
    }

    for (int i = 0; i < Loop; ++i) {
        UI[i] = List[i]; // 외부에서 사용하기 위해 저장

        // nullptr이면 무시
        if (UI[i] == nullptr) {
            return;
        }
    }

    if (!Slots.Num()) {
        Slots.Init(nullptr,
            [](const TArray<USlotUI*>& In) {
                int Loop = 0;
                for (int i = 0; i < In.Num(); ++i)
                    Loop += In[i]->GetSlotCount();
                return Loop;
            }(UI)
        );
    }
}
