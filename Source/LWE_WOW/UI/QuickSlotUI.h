// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Manager/UIManager.h>

#include "CoreMinimal.h"
#include "LWE_WOW/UI/SlotUI.h"
#include "QuickSlotUI.generated.h"

class AGenericInput;

/**
 * 
 */
UCLASS()
class LWE_WOW_API UQuickSlotUI : public USlotUI
{
	GENERATED_BODY()

protected:
    inline static int SLOT_MAX = 0;

public:
    virtual void   NativeConstruct        (                                                            ) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&                      ) override;
    virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&                      ) override;
    virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
    virtual bool   NativeOnDrop           (const FGeometry&, const FDragDropEvent&, UDragDropOperation*) override;
    virtual void   NativeOnDragCancelled  (                  const FDragDropEvent&, UDragDropOperation*) override;

public:
    void           SetSlotInfo(EActionID, UGenericSkill*);
    UGenericSkill* GetSlotInfo(EActionID);

public:
    // Index를 ActionID로 바꿉니다.
    // 이 슬롯이 1번 슬롯이고 Index 가 1이면
    // 12칸 슬롯 기준으로 13이 반환되게 됩니다.
    EActionID GetSlotID(int);

public:
    // ActionID를 Index로 바꿉니다.
    // ID가 37인 경우 12칸 슬롯 기준으로 [2][1] 배열과 동일한 형태가 됩니다.
    // 따라서 1이 반환되게 됩니다.
    static int GetSlotIndex(EActionID);

public:
    // ActionID를 WidgetIndex로 바꿉니다.
    // ID가 14인 경우 12칸 슬롯 기준으로 [1][2] 배열과 동일한 형태가 됩니다.
    // 따라서 1이 반환되게 됩니다.
    static UUIManager::EUIList GetUIIndex(EActionID);

public:
    int WidgetIndex = 0;
};