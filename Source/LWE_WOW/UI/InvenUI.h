#pragma once

#include <LWE_WOW/Data/ItemData.h>

#include "CoreMinimal.h"
#include "SlotUI.h"
#include "InvenUI.generated.h"

UCLASS(Blueprintable)
class UInvenUI : public USlotUI
{
	GENERATED_BODY()

    friend class UPlayerManager;

public:
    virtual void   NativeConstruct        (                                                            ) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&                      ) override;
    virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&                      ) override;
    virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
    virtual bool   NativeOnDrop           (const FGeometry&, const FDragDropEvent&, UDragDropOperation*) override;
    virtual void   NativeOnDragCancelled  (                  const FDragDropEvent&, UDragDropOperation*) override;
	
public:
    Slot* GetEmptySlot();

public:
	void AddItem(const FItemData&);
    void RemoveItem(Slot*);
};