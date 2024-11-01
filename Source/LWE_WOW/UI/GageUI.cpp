#include "GageUI.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Manager/UIManager.h>



void UGageUI::NativeConstruct()
{
    Super::NativeConstruct();

    // 첫 번째 UI위젯을 게이지로 판단합니다.
    UPanelWidget* Panel = UUIManager::GetRoot(this);
    m_Bar = Cast<UProgressBar>(Panel->GetChildAt(0));
    if (!m_Bar) {
        // 없으면 찾습니다.
        int Loop = Panel->GetChildrenCount();
        for (int i = 1; i < Loop; ++i) {
            m_Bar = Cast<UProgressBar>(Panel->GetChildAt(i));
            if (m_Bar) return;
        }
        check(m_Bar); // 체크
    }
    Hide();
}

void UGageUI::Update(float InCurrent, float InMax, float InTimer)
{
    m_Bar->SetVisibility(ESlateVisibility::Visible);
    m_Bar->SetPercent(InCurrent / InMax);

    Show(InTimer);
}

void UGageUI::OnTick(float DeltaTime)
{
    // 타이머입니다.
    if (m_Timer > 0) {
        m_Timer -= DeltaTime;
        if (m_Timer <= 0) {
            Hide();
        }
    }
}

void UGageUI::Hide()
{
    // 숨기는 건 타이머 상관이 없으나 OnTick 함수를 비활성화 하기 위해 할당합니다.
    m_Timer = 0;
    m_Bar->SetVisibility(ESlateVisibility::Hidden);
}

void UGageUI::Show(float InTimer)
{
    m_Bar->SetVisibility(ESlateVisibility::Visible);
    // 타이머가 있어야 OnTick이 활성화 됩니다. 
    if(InTimer) {
        m_Timer = InTimer;
    }
    else m_Timer = 0;
}