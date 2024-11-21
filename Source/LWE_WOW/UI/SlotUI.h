#pragma once

#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Generic/GenericUI.h>
#include <LWE_WOW/UI/SlotInfo.h>

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
        USlot*  Info;
        UImage* Icon;
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
    void  SetSlot(USlot*, int);
    void  Remove(int);

public:
    void Swap(Slot* A, Slot* B);

public:
    // Select 해제합니다.
    void SelectedFree();

public:
    // 현재 마우스 위치의 슬롯을 가져옵니다.
    Slot* GetHoveredSlot(const FPointerEvent&);

    // 현재 마우스 위치의 슬롯을 가져오고 저장합니다.
    Slot* SetHoveredSlot(const FPointerEvent&);

public:
    int GetSlotCount() const;

protected:
    AGenericCharacter* m_Parent;
    AGenericInput*     m_Input;

public:
    // 실제 슬롯이 담긴 패널 (최상위 자식)
    UPanelWidget* SlotWidget = nullptr;
        
protected:
    // 슬롯의 부모 == this
    // 이 변수를 사용하기 위해 Payload를 쓰지 않습니다.
    // 마우스는 한 개라고 가정하기 때문에 static으로 사용합니다.
    inline static USlotUI*  st_SelectedParent   = nullptr;
    inline static Slot*     st_Selected         = nullptr;

protected:
    // 투명 텍스처입니다.
    inline static UTexture2D* EMPTY = nullptr;
};