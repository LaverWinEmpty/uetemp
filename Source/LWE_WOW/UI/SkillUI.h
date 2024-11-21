#pragma once

#include "CoreMinimal.h"
#include "SlotUI.h"
#include "SkillUI.generated.h"

class AGenericCharacter;
class USkillData;

// 원래 캐릭터마다 다른 스킬창이여야 하나 대충 구현

UCLASS(Blueprintable)
class USkillUI : public USlotUI
{
	GENERATED_BODY()

public:
    USkillUI(const FObjectInitializer&);

public:
	virtual void   NativeConstruct        (                                                            ) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&                      ) override;
    virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&                      ) override;
    virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
    virtual bool   NativeOnDrop           (const FGeometry&, const FDragDropEvent&, UDragDropOperation*) override;
    virtual void   NativeOnDragCancelled  (                  const FDragDropEvent&, UDragDropOperation*) override;

public:
    void RegisterSkill();

private:
    static const TCHAR* const PATH[];
};
