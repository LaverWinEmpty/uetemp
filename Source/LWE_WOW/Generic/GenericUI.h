#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SCompoundWidget.h"
#include "SlateCore.h"
#include "Slate.h"
#include "GenericUI.generated.h"

UCLASS(Blueprintable)
class UGenericUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UGenericUI(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order")
	int32 ViewportZOrder = 0;

public:
	// 이렇게 쓰려고 UserWidget 확장
	inline void AddToViewportWithPresetZOrder() {
		UUserWidget::AddToViewport(ViewportZOrder);
	}
};