#pragma once

#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Generic/GenericUI.h>

#include "SlotUI.generated.h"

class AGenericCharacter;

// 슬롯을 소유한 UI들의 베이스 클래스 입니다.

UCLASS(Blueprintable)
class USlotUI : public UGenericUI
{
	GENERATED_BODY()

protected:
    struct Slot {
        UPROPERTY()
        UObject* Info; // 슬롯의 실제 정보입니다.
        UImage*  Icon; // 슬롯의 이미지 입니다.
    };
    TArray<Slot> m_Slots;

public:
	USlotUI(const FObjectInitializer&);

public:
    virtual void   NativeConstruct        (                                                            ) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&                      ) override;
    virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&                      ) override;
    virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
    virtual bool   NativeOnDrop           (const FGeometry&, const FDragDropEvent&, UDragDropOperation*) override;
    virtual void   NativeOnDragCancelled  (                  const FDragDropEvent&, UDragDropOperation*) override;

public:
    template<typename T> T* Get(int Idx) { return Cast<T>(m_Slots[Idx].Info); }

public:
    Slot* GetSlot(int);
    void  SetSlot(const Slot&, int);
    void  Remove(int);

public:
    void Swap(Slot* A, Slot* B);

public:
    virtual void Select(Slot& In, const FPointerEvent& InEvent);
    virtual void Release();

public:
    // 현재 마우스 위치의 슬롯을 가져옵니다.
    Slot* GetHoveredSlot(const FPointerEvent&);

    // 현재 마우스 위치의 슬롯을 가져오며 Selected에 저장해둡니다.
    Slot* SetHoveredSlot(const FPointerEvent&);

protected:
    AGenericCharacter* m_Parent;

public:
    // 실제 슬롯이 담긴 패널 (최상위 자식)
    UPanelWidget* SlotWidget = nullptr;

protected:
    inline static Slot*     st_Selected         = nullptr;  // 선택된 슬롯 정보
    inline static USlotUI*  st_SelectedParent   = nullptr;  // 슬롯의 부모 == this
    inline static FVector2d st_SelectedMousePos = { 0, 0 }; // 선택 시점의 마우스 위치

public:
    // 투명 텍스처입니다.
    static UTexture2D* const& EMPTY;
};