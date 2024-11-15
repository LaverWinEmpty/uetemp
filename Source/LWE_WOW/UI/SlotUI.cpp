#include "SlotUI.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

#include <LWE_WOW/Manager/UIManager.h>

// UTexture2D* const& USlotUI::EMPTY = ConstructorHelpers::FObjectFinder<UTexture2D>(_T("/Game/Resource/UI/Empty64.Empty64")).Object;

UTexture2D* const& USlotUI::EMPTY = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, _T("/Game/Resource/UI/Empty64.Empty64")));

USlotUI::USlotUI(const FObjectInitializer& In) : Super(In)
{
    check(EMPTY);
}

void USlotUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (UPanelWidget* Panel = UUIManager::GetRoot(this)) {
        SlotWidget = Cast<UPanelWidget>(Panel->GetChildAt(0)); // 슬롯 패널은 첫번째 자식으로 고정
        check(SlotWidget); // 검사

        int Loop = SlotWidget->GetChildrenCount();
        m_Slots.Init({ nullptr, nullptr }, Loop);

        for (int32 i = 0; i < Loop; ++i) {
            if (UImage* Image = Cast<UImage>(SlotWidget->GetChildAt(i))) {
                m_Slots[i].Icon = Image;                     // 자식을 이미지로 변경 후가져옵니다.
                m_Slots[i].Icon->SetBrushFromTexture(EMPTY); // 빈 이미지 적용
            }

            else ensure(false); // 이미지가 아닌 자식
        } // end for
    } // end if
    else check(false);

    m_Parent = Cast<AGenericCharacter>(GetOwningPlayerPawn());
}

FReply USlotUI::NativeOnMouseButtonDown(const FGeometry& In, const FPointerEvent& InEvent)
{
    if (InEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
        Slot* Got = SetHoveredSlot(InEvent);
        if (Got == nullptr) {
            return FReply::Handled();
        }

        if (Got->Info == nullptr) {
            return FReply::Handled();
        }

        // 드래그 준비
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return FReply::Unhandled();
}

FReply USlotUI::NativeOnMouseButtonUp(const FGeometry& In, const FPointerEvent& InEvent)
{
    // set nullptr
    Release();
    return FReply::Handled();
}

void USlotUI::NativeOnDragDetected(const FGeometry& In,
                                   const FPointerEvent& InEvent,
                                   UDragDropOperation*& Out)
{
    UImage* Image = NewObject<UImage>(this);
    Image->SetBrush(st_Selected->Icon->GetBrush());

    // 화면상 아이콘 위치
    FVector2D IconPos = st_Selected->Icon->GetCachedGeometry().
        GetLocalPositionAtCoordinates(FVector2d::ZeroVector);

    // 실제 위젯 위치
    FVector2D WidgetPos = st_SelectedParent->SlotWidget->GetCachedGeometry().
        GetLocalPositionAtCoordinates(FVector2d::ZeroVector);

    // 계산
    Image->SetRenderTranslation(IconPos + WidgetPos);

    Out = NewObject<UDragDropOperation>();
    Out->DefaultDragVisual = Image;
    Out->Pivot = EDragPivot::MouseDown;
}

bool USlotUI::NativeOnDrop(const FGeometry& In, const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
    Release();
    return true;
}

void USlotUI::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Release();
}

auto USlotUI::GetSlot(int Index)->Slot*
{
    return &m_Slots[Index];
}

void USlotUI::SetSlot(const Slot& InSlot, int InIndex)
{
    m_Slots[InIndex] = InSlot;
}

void USlotUI::Remove(int InIndex)
{
    m_Slots[InIndex].Icon->SetBrushFromTexture(EMPTY);
    m_Slots[InIndex].Info = nullptr;
}

void USlotUI::Swap(Slot* A, Slot* B)
{
    // 동일한 위치면 무시
    if (!A || !B || A == B) return;

    FSlateBrush AIconTempBrush = A->Icon->GetBrush();
    UObject* AInfo = A->Info;

    A->Info = B->Info;
    A->Icon->SetBrush(B->Icon->GetBrush());

    B->Info = AInfo;
    B->Icon->SetBrush(AIconTempBrush);
}


void USlotUI::Select(Slot& In, const FPointerEvent& InEvent)
{
    st_Selected         = &In;
    st_SelectedParent   = this;
    st_SelectedMousePos = InEvent.GetScreenSpacePosition();
}

void USlotUI::Release()
{
	st_Selected       = nullptr;
    st_SelectedParent = nullptr;
}

auto USlotUI::SetHoveredSlot(const FPointerEvent& InEvent)->Slot*
{
    st_Selected = GetHoveredSlot(InEvent);
    st_SelectedParent = this;
    st_SelectedMousePos = InEvent.GetScreenSpacePosition();
    return st_Selected;
}

auto USlotUI::GetHoveredSlot(const FPointerEvent& InEvent)->Slot*
{
    int Loop = m_Slots.Num();
    for (int i = 0; i < Loop; ++i) {
        // IsHovered 오작동, 마우스 위치로 처리
        FVector2D MousePos = InEvent.GetScreenSpacePosition();

        // 반환
        if (m_Slots[i].Icon->GetCachedGeometry().IsUnderLocation(MousePos)) {
            return &m_Slots[i];
        }
    }

    return nullptr;
}