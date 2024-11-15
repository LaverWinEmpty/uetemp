#include "Slotted.h"

#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

ISlotted::ISlotted()
{
    static ConstructorHelpers::FObjectFinder<UTexture2D> Finder =
        _T("/Game/Resource/UI/Empty64.Empty64");
    if (!m_Empty) {
        m_Empty = Finder.Object;
    }
}

bool ISlotted::IsSameTypeUI(ISlotted* In) const
{
	return GetType() == In->GetType();
}

void ISlotted::Select(UImage* InImage, const FPointerEvent& InEvent, int InIndex)
{
    st_Selected         = InImage;                               // 이미지 저장
    st_SelectedMousePos = InEvent.GetScreenSpacePosition();      // 마우스 위치 저장
    st_SelectedParent   = this;                                  // 아이콘의 부모 즉 this
}

void ISlotted::Release()
{
    st_Selected = nullptr;
    st_SelectedParent = nullptr;
}
