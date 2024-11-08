#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

#include "LWE_WOW/Generic/GenericIcon.h"

UUIManager::UUIManager()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> Finder[] = {
        (_T("/Game/UI/UI_Main.UI_Main_C")),
        (_T("/Game/UI/UI_SkillSlot.UI_SkillSlot_C")),
    };

    static constexpr int SIZE = (sizeof(Finder) / sizeof(*Finder));
    
    Resource.Init(nullptr, SIZE);
    for (int i = 0; i < SIZE; ++i) {
        Resource[i] = Finder[i].Class;
    }
}

void UUIManager::Setup()
{
    if (m_IsSetup) return;
    m_IsSetup = true;

    int Loop = Resource.Num();
    for (int i = 0; i < UI_END; ++i) {
        Widgets[i] = CreateWidget<UUserWidget>(GetWorld(), Resource[i]);
        Widgets[i]->AddToViewport();
    }

    UPanelWidget* Root, * Panel;
    UButton* Btn;

    Root = UUIManager::GetRoot(Widgets[UI_MAIN]);

    Panel = Cast<UPanelWidget>(Root->GetChildAt(0));
    Btn = Cast<UButton>(Panel->GetChildAt(0));
    Btn->OnClicked.AddDynamic(this, &UUIManager::ExitButton);


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
}

void UUIManager::Cleanup()
{
    for (int i = 0; i < UI_END; ++i) {
        Widgets[i] = nullptr;
    }
    m_IsSetup = false;
    FTimerManager& TimerManager = GetWorld()->GetTimerManager();
    TimerManager.ClearTimer(MessageBoxStarter);
    TimerManager.ClearTimer(MessageBoxUpdater);
}

void UUIManager::Reset()
{
    Cleanup();
    Setup();
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

