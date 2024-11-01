#include "GenericIcon.h"
#include "Components/CanvasPanelSlot.h"
#include "Input/Reply.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericInput.h>
#include <LWE_WOW/Manager/UIManager.h>

//UWidget* UGenericIcon::Get(const TCHAR* const InName)
//{
//}

UGenericIcon::UGenericIcon(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> Finder =
        _T("/Script/Engine.Texture2D'/Engine/EngineResources/Bad.Bad'");
    check(Finder.Object);
    Empty = Finder.Object;
}

void UGenericIcon::NativeConstruct()
{
    Super::NativeConstruct();

    int Index = 0;

    if (UPanelWidget* Panel = UUIManager::GetRoot(this)) {
        int Loop = Panel->GetChildrenCount();
        for (int32 i = 0; i < Loop; ++i) {
            if (UImage* Image = Cast<UImage>(Panel->GetChildAt(i))) { // 자식을 이미지로 변환 후
                if (Image->GetName().StartsWith(_T("Slot_"))) {       // 슬롯이라고 판단 되면
                    Slot[Index++] = Image;                            // 저장
                    if (Index >= SLOT_MAX) {                          // 슬롯 초과 방지
                        break;
                    } // end if
                } // end if
            } // end if
        } // end for
    } // end if

    SlotCount = Index;

    if (APawn* Pawn = GetOwningPlayerPawn()) {
        m_Controller = Cast<AGenericInput>(Pawn->GetController());
    }

    //if (UWidget* Root = GetRootWidget()) {
    //    if (UPanelWidget* Panel = Cast<UPanelWidget>(Root)) {
    //        for (int32 i = 0; i < Panel->GetChildrenCount(); ++i) {
    //            if (UWidget* Child = Panel->GetChildAt(i)) {
    //                if (UImage* Image = Cast<UImage>(Child)) {
    //                    if (Image->GetName().StartsWith(_T("Slot_"))) {
    //                        if (Index < SLOT_MAX) Slot[Index++] = Image;
    //                        else break;
    //                    } // end if
    //                } // end if
    //            } // end if
    //        } // end for
    //    } // end if
    //} // end if

    // 빈 이미지 적용
    for (int i = 0; i < Index; ++i) {
        Slot[i]->SetBrushFromTexture(Empty);
    }
}

FReply UGenericIcon::NativeOnMouseButtonDown(const FGeometry& In, const FPointerEvent& InEvent)
{
    for (int i = 0; i < SlotCount; ++i) {
        if (Slot[i]->IsHovered()) {
            break;
        }
    }

    return FReply::Handled();
}

FReply UGenericIcon::NativeOnMouseButtonUp(const FGeometry& In, const FPointerEvent& InEvent)
{
    // 기본 행동
    FReply Reply = Super::NativeOnMouseButtonDown(In, InEvent);

    for (int i = 0; i < SlotCount; ++i) {
        if (Slot[i]->IsHovered()) {
            // 슬롯 매핑 해줘야 할듯
            // 임시 코드
            m_Controller->PlayerAction(static_cast<EActionID>(IA_COMMAND_0 + i));
            break;
        }
    }

    // 드래그 등 이벤트 중이 아니면 Handled()를 반환합니다.
    return Reply.IsEventHandled() ? Reply : FReply::Handled();
}

void UGenericIcon::NativeOnMouseEnter(const FGeometry& In, const FPointerEvent& InEvent)
{
    Super::NativeOnMouseEnter(In, InEvent);
}

void UGenericIcon::NativeOnMouseLeave(const FPointerEvent& InEvent)
{
    Super::NativeOnMouseLeave(InEvent);
}

FReply UGenericIcon::NativeOnMouseMove(const FGeometry& In, const FPointerEvent& InEvent)
{
    FReply Reply = Super::NativeOnMouseMove(In, InEvent);

    FVector2D MousePosition = InEvent.GetScreenSpacePosition();

    // 모든 슬롯을 체크
    for (int i = 0; i < SlotCount; ++i) {
        if (Slot[i]->GetCachedGeometry().IsUnderLocation(MousePosition)) {
            return FReply::Handled();
        }
    }

    return Reply.IsEventHandled() ? Reply : FReply::Handled();
}


//void UGenericIcon::OnGeometryChanged(const FGeometry& OldGeometry)
//{
//    Super::OnGeometryChanged(OldGeometry);
//
//    //// 배치 완료 후 슬롯 사이즈를 가져오기
//    //UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
//    //if (CanvasSlot)
//    //{
//    //    FVector2D Size = CanvasSlot->GetSize();
//    //    UE_LOG(LogTemp, Warning, TEXT("Slot Size (OnGeometryChanged): X = %f, Y = %f"), Size.X, Size.Y);
//
//    //    // 필요한 로직 수행 후 이벤트 비활성화(선택적)
//    //    SetVisibility(ESlateVisibility::HitTestInvisible); // 예시
//    //}
//}