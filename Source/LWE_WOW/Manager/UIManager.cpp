#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

#include <LWE_WOW/UI/QuickSlotUI.h>
#include <LWE_WOW/Common/Util.h>

UUIManager::UUIManager()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> Finder[] = {
        (_T("/Game/UI/UI_Main.UI_Main_C")),
        (_T("/Game/UI/UI_Inventory.UI_Inventory_C")),
        (_T("/Game/UI/UI_Equipment.UI_Equipment_C")),
        (_T("/Game/UI/UI_Skill.UI_Skill_C")),
        (_T("/Game/UI/UI_QuickSlot_0.UI_QuickSlot_0_C")),
        (_T("/Game/UI/UI_QuickSlot_1.UI_QuickSlot_1_C")),
    };

    static constexpr int SIZE = (sizeof(Finder) / sizeof(*Finder));
    Resource.Init(nullptr, SIZE);
    for (int i = 0; i < SIZE; ++i) {
        Resource[i] = Finder[i].Class;
    }
}

void UUIManager::Setup()
{
    if (m_IsSetup) {
        Cleanup();
    }
    m_IsSetup = true;

    int Loop = Resource.Num();

    // 이곳에서 할당됩니다.
    for (int i = UI_BEGIN; i < UI_END; ++i) {
        Widgets[i] = CreateWidget<UGenericUI>(GetWorld(), Resource[i]);
        Widgets[i]->AddToViewportWithPresetZOrder();
    }

    // 퀵슬롯 인덱싱
    int Index = 0;
    for (int i = UI_QUICKSLOT_0; i < UI_QUICKSLOT_END; ++i) {
        Cast<UQuickSlotUI>(Widgets[i])->WidgetIndex = Index++;
    }

    // 버튼 할당 관련

    UPanelWidget* Root, * Panel;
    UButton* Btn;

    Root = UUIManager::GetRoot(Widgets[UI_MAIN]);

    // 나가기 UI
    Panel = Cast<UPanelWidget>(Root->GetChildAt(0));
    Btn = Cast<UButton>(Panel->GetChildAt(0));
    Btn->OnClicked.AddDynamic(this, &UUIManager::ExitButton);

    // 종료 UI
    Panel = Cast<UPanelWidget>(Root->GetChildAt(1));
    Btn = Cast<UButton>(Panel->GetChildAt(0));
    Btn->OnClicked.AddDynamic(this, &UUIManager::YesButton);
    Btn = Cast<UButton>(Panel->GetChildAt(1));
    Btn->OnClicked.AddDynamic(this, &UUIManager::NoButton);

    // 종료 다이얼로그
    m_Dialog_Exit = Cast<UWidget>(Panel);

    // on / off
    m_bShowExitDialog = true;
    HideExitDialogBox();

    Panel = Cast<UPanelWidget>(Root->GetChildAt(4));
    m_DeadOverride = Cast<UImage>(Panel->GetChildAt(0));

    m_DeadOverride->SetVisibility(ESlateVisibility::Hidden); // 죽으면 회색빛으로 override, 숨김

    m_MsgBox = Widgets[UI_MAIN]->WidgetTree->FindWidget<UTextBlock>("MessageBox");

    // 초기값: 숨김
    Show(UI_INVENTORY, false);
    Show(UI_EQUIPMENT, false);
    Show(UI_SKILL,     false);
}

void UUIManager::Cleanup()
{
    for (int i = 0; i < UI_END; ++i) {
        Widgets[i]->MarkAsGarbage();
    }
    m_IsSetup = false;
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.ClearTimer(MessageBoxStarter);
    TimerManager.ClearTimer(MessageBoxUpdater);
}

UPanelWidget* UUIManager::GetRoot(UUserWidget* In)
{
    if (UWidget* Root = In->GetRootWidget()) {
        return Cast<UPanelWidget>(Root);
    }
    return nullptr;
}

UUIManager* UUIManager::Instance(UObject* InWorldContextObject)
{
    if (UGameInstance* GI = UGameplayStatics::GetGameInstance(InWorldContextObject->GetWorld())) {
        if (UUIManager* Self = GI->GetSubsystem<UUIManager>()) {
            return Self;
        }
    }
    return nullptr;
}

void UUIManager::SetTargetInfo(AActor* In)
{
    UTextBlock* TextBlock = Widgets[UI_MAIN]->WidgetTree->FindWidget<UTextBlock>("Target");

    if (TextBlock) {
        FText Text;
        if (In) Text = FText::FromString(In->GetName());
        TextBlock->SetText(Text);
    }
}

UQuickSlotUI* UUIManager::GetSkillSlot(EActionID In)
{
    return Cast<UQuickSlotUI>(Widgets[UQuickSlotUI::GetUIIndex(In)]);
}

void UUIManager::Show(EUIList InIndex, bool Is)
{
    // 최상위 패널 기준이기 때문에 Visible이 아닌 해당 옵션을 사용합니다.
    Widgets[InIndex]->SetVisibility(Is ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UUIManager::Toggle(EUIList InIndex)
{
    // 예외처리할 목록
    switch (InIndex) {
        case UI_END:
            // Insert...
            return;
    }

    ESlateVisibility State = Widgets[InIndex]->GetVisibility();
    if (State == ESlateVisibility::SelfHitTestInvisible) {
        State = ESlateVisibility::Hidden;
    }
    else if (State == ESlateVisibility::Hidden) {
        State = ESlateVisibility::SelfHitTestInvisible;
    }

    // 다른 상태면 그냥 숨겨버립니다.
    else State = ESlateVisibility::Hidden;

    if (Widgets[InIndex]) {
        Widgets[InIndex]->SetVisibility(State);
    }
}

void UUIManager::HideExitDialogBox()
{
    if (!m_bShowExitDialog) {
        return;
    }
    m_bShowExitDialog = false;  
    m_Dialog_Exit->SetVisibility(ESlateVisibility::Hidden);
}

void UUIManager::ShowMessageBox()
{
    if (m_bShowExitDialog) {
        return;
    }
    m_bShowExitDialog = true;
    m_Dialog_Exit->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::ExitButton()
{
    ShowMessageBox();
}

void UUIManager::NoButton()
{
    HideExitDialogBox();
}

void UUIManager::YesButton()
{
    Cleanup();
    UGameplayStatics::OpenLevel(GetWorld(), _T("LobbyMap"));
}

void UUIManager::OnDead()
{
    m_DeadOverride->SetVisibility(ESlateVisibility::Visible);
}

void UUIManager::OnResurrect()
{
    m_DeadOverride->SetVisibility(ESlateVisibility::Hidden);
}

void UUIManager::SetMessageText(const TCHAR* const InString)
{
    if (!m_MsgBox || !InString) {
        ensure(false);
        return;
    }

    TArray<FString> Lines;

    // Get text
    FString Text = m_MsgBox->GetText().ToString();

    //배열로 분할 후 배열에 새 문자열 추가
    Text.ParseIntoArray(Lines, _T("\n"));
    Lines.Add(InString);

    // 4줄 이상이면 가장 오래된 줄 제거
    if (Lines.Num() > 4) {
        Lines.RemoveAt(0);
    }

    // 배열을 다시 결합 후 문자열 세팅
    Text = FString::Join(Lines, _T("\n"));
    m_MsgBox->SetText(FText::FromString(Text));

    // 투명도 초기화
    m_MsgBox->SetRenderOpacity(1.0f);

    FTimerManager& TimerManager = GetWorld()->GetTimerManager();

    // 기존 타이머가 활성화된 경우 모두 초기화
    TimerManager.ClearTimer(MessageBoxStarter);
    TimerManager.ClearTimer(MessageBoxUpdater);

    // SetTimer(Handle, Lamda, 3,   false) << 3초 후 람다를 호출합니다.
    // SetTimer(Handle, Lamda, 0.1, true)  << 0.1초마다 람다를 호출합니다. (람다)
    TimerManager.SetTimer(MessageBoxStarter, [this, &TimerManager]() {
        TimerManager.SetTimer(MessageBoxUpdater, [this]() {
             float Opacity = m_MsgBox->GetRenderOpacity() - 0.2; // 1초당 0.2
            if (Opacity <= 0) {
                Opacity = 0;
            }
            m_MsgBox->SetRenderOpacity(Opacity);
            if (Opacity <= 0.0f) {
                GetWorld()->GetTimerManager().ClearTimer(MessageBoxUpdater); // 타이머 정지
                m_MsgBox->SetText(FText::FromString( _T("")));               // 텍스트 비우기
            }
        }, 0.1f, true); // 0.1초 간격으로 반복
    }, 3.0f, false); // 3초 뒤 1회 호출
}


//void UUIManager::OnResizeViewport()
//{

//// 화면 사이즈를 가져옵니다
//GEngine->GameViewport->GetViewportSize(Screen);

//// 스킬슬롯 기반으로 생성
//for (int i = UI_SKILL_SLOT_BEGIN; i < UI_SKILL_SLOT_2; ++i) {
//    Widgets[i] = CreateWidget<UUserWidget>(GetWorld(), Source[i]);
//}

//// 최종적으로 생성된 위젯들을 뷰포트에 추가
//for (int i = UI_SKILL_SLOT_BEGIN; i < UI_SKILL_SLOT_2; ++i) {
//    Widgets[i]->AddToViewport();
//    Widgets[i]->ForceLayoutPrepass();
//}

//// UI 위치를 바꿀 수 있게(미구현) 수동 제어
//// 임시값
//FVector2d Position;
//Position.X = (Screen.X - Widgets[UI_SKILL_SLOT_1]->GetDesiredSize().X * 0.5) * 0.5;
//Position.Y =  Screen.Y - Widgets[UI_SKILL_SLOT_1]->GetDesiredSize().Y * 0.5;
//Widgets[UI_SKILL_SLOT_1]->SetPositionInViewport(Position);


//FVector2d NewScreenSize;

//// 화면 사이즈를 가져옵니다
//GEngine->GameViewport->GetViewportSize(NewScreenSize);

//// 1920 / 1080 기준으로 UI 만든다고 가정
//// 현재 화면의 비율을 가져옵니다.
//Screen = { NewScreenSize.X / Screen.X, NewScreenSize.Y / Screen.Y };

//// 해당 화면 비율로 모든 UI를 업데이트합니다.
//for (int i = UI_SKILL_SLOT_BEGIN; i < UI_SKILL_SLOT_2; ++i) {
//    Widgets[i]->SetRenderScale(Screen);
//}

//Screen = NewScreenSize;
//}

//UCanvasPanelSlot* UUIManager::Widget::Canvas()
//{
//    if (!Default) {
//        Default = Source->GetDefaultObject<UUserWidget>();
//    }
//
//    // UPanelSlot
//    return Cast<UCanvasPanelSlot>(Default->Slot);
//    //FVector2D examplePanelSize;
//    //if (NULL != exampleCanvasPanelSlot) {
//    //    examplePanelSize = exampleCanvasPanelSlot->GetSize();
//    //}
//
//
//
//
//
//
//    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Default->Slot);
//    if (CanvasSlot) {
//        __debugbreak();
//    }
//    __debugbreak();
//    return Cast<UCanvasPanelSlot>(Default->Slot);
//}

