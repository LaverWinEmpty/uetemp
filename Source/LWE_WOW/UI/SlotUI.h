#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SCompoundWidget.h"
#include "SlateCore.h"
#include "Slate.h"

#include "Components/Image.h"

#include "SlotUI.generated.h"

class UImage;
class AGenericInput;

UCLASS(Blueprintable)
class LWE_WOW_API USlotUI : public UUserWidget
{
    GENERATED_BODY()

public:
    static constexpr int SLOT_MAX = 50;

public:
    UImage* Slot[SLOT_MAX];
    int     SlotCount;

public:
    UWidget* Get(const TCHAR* const);

public:
    USlotUI(const FObjectInitializer&);

public:
    virtual void NativeConstruct() override;

public:
    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;
    virtual FReply NativeOnMouseButtonUp  (const FGeometry&, const FPointerEvent&) override;
    virtual void   NativeOnMouseEnter     (const FGeometry&, const FPointerEvent&) override;
    virtual void   NativeOnMouseLeave     (                  const FPointerEvent&) override;
    virtual FReply NativeOnMouseMove      (const FGeometry&, const FPointerEvent&) override;

private:
    UTexture2D* Empty;

private:
    AGenericInput* m_Controller;

    //virtual void   NativeOnDragDetected   (const FGeometry&, const FPointerEvent&, UDragDropOperation*&) override;
    //virtual FReply NativeOnMouseMove      (const FGeometry&, const FPointerEvent&                      ) override;

};