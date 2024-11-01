#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetComponent.h"

#include "GageUI.generated.h"
// Temp->SetRelativeLocation({ 0, 0, 120 });

class UImage;
class AGenericCharacter;

UCLASS(Blueprintable)
class LWE_WOW_API UGageUI : public UUserWidget
{
    GENERATED_BODY()

public:
    void NativeConstruct();

public:
    static void Setup(UWidgetComponent*, const TCHAR* const InPath);

public:
    void OnTick(float DeltaTime);

public:
    // 게이지를 업데이트 합니다.
    // 타이머 값이 있으면 해당 타이머 시간 동안만 보여줍니다.
    void Update(float InCurrent, float InMax, float InTimer = 0);

    // 게이지를 보여줍니다.
    // 타이머 값이 있으면 해당 타이머 시간 동안만 보여줍니다.
    void Show(float InTimer = 0);

    // 게이지를 숨깁니다.
    void Hide();

private:
    UProgressBar* m_Bar;

    float m_Timer;
};

inline void UGageUI::Setup(UWidgetComponent* InParent, const TCHAR* const InPath)
{
    ConstructorHelpers::FClassFinder<UUserWidget>Finder = { InPath };

    InParent->SetWidgetClass(Finder.Class);
    InParent->SetWidgetSpace(EWidgetSpace::Screen);
    InParent->SetDrawSize({ 200, 20 });
    InParent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}