#pragma once

#include "CoreMinimal.h"
#include "Slotted.generated.h"

class UCanvasPanelSlot;
class UImage;

UINTERFACE(MinimalAPI) class USlotted : public UInterface { GENERATED_BODY() };

// 슬롯의 유형
enum class ESlotType {
    NONE,
    SHORTCUT,       // 단축키
    SKILLTREE,      // 스킬트리
    ITEM_EQUIPOED,  // 장비창
    ITEM_INVENTORY, // 인벤토리

    // 기타 구매창이라든가...
};

class LWE_WOW_API ISlotted
{
	GENERATED_BODY()

public:
    ISlotted();

public:
    virtual ESlotType GetType() const = 0;
    
public:
    bool IsSameTypeUI(ISlotted*) const;

public:
    void Select(UImage* InImage, const FPointerEvent& InEvent, int InIndex);
    static void Release();

protected:
    inline static UImage*           st_Selected = nullptr;
    inline static UCanvasPanelSlot* st_SelectedSlot = nullptr;
    inline static FVector2d         st_SelectedPos = { 0, 0 };
    inline static FVector2d         st_SelectedMousePos = { 0, 0 };
    inline static ISlotted*         st_SelectedParent = nullptr;

    // 투명 텍스처입니다.
private:
    inline static UTexture2D* m_Empty = nullptr;

    // const 목적 reference: 외부에서 읽을 때 사용하는 변수 
public:
    inline static UTexture2D* const& EMPTY = m_Empty;
};