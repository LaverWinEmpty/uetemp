#pragma once

#include "CoreMinimal.h"
#include "SlotUI.h"
#include "EquipUI.generated.h"

class UTextBlock;

UCLASS(Blueprintable)
class UEquipUI : public USlotUI
{
	GENERATED_BODY()

public:
    virtual void   NativeConstruct        (                                                            ) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&                      ) override;
    virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&                      ) override;
    virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
    virtual bool   NativeOnDrop           (const FGeometry&, const FDragDropEvent&, UDragDropOperation*) override;
    virtual void   NativeOnDragCancelled  (                  const FDragDropEvent&, UDragDropOperation*) override;

public:
    TMap<Slot*, EItemSlotTypeIdx> Indexer;

public:
    UTextBlock* PlayerStatus;

public:
	void Equip(Slot* This, Slot* From);
    void Equip(EItemSlotTypeIdx, Slot*);

public:
    void UpdatePlayerStatus();
};