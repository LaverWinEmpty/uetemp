#include "EquipUI.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

#include <LWE_WOW/UI/InvenUI.h>
#include <LWE_WOW/Generic/GenericItem.h>
#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Manager/UIManager.h>

void UEquipUI::NativeConstruct()
{
	Super::NativeConstruct();

	int Loop = m_Slots.Num();

	for (int i = 0; i < Loop; ++i) {
		// 0은 NONE => 1부터 시작
		Indexer.Add(&m_Slots[i], static_cast<EItemSlotTypeIdx>(i + 1));
	}

	PlayerStatus = WidgetTree->FindWidget<UTextBlock>(_T("PlayerStatus"));
	UpdatePlayerStatus();
}

FReply UEquipUI::NativeOnMouseButtonDown(const FGeometry& In, const FPointerEvent& InEvent)
{
	if (InEvent.GetEffectingButton() == EKeys::RightMouseButton) {

		// 인벤토리 UI 가져와서
		// 빈 슬롯 찾아서 그 슬롯과 교체, 빈 슬롯 없으면 무시
		if (UInvenUI* UI = Cast<UInvenUI>(UUIManager::Instance(this)->Widgets[UUIManager::UI_INVENTORY])) {
			if (Slot* This = GetHoveredSlot(InEvent)) {
				Cast<UGenericItem>(This->Info)->OnUnequipment(m_Parent); // 장착 해제
				Swap(This, UI->GetEmptySlot());
			}
			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonDown(In, InEvent);
}

FReply UEquipUI::NativeOnMouseButtonUp(const FGeometry& In, const FPointerEvent& InEvent)
{
	// 무시
	return Super::NativeOnMouseButtonUp(In, InEvent);
}

void UEquipUI::NativeOnDragDetected(const FGeometry& In, const FPointerEvent& InEvent, UDragDropOperation*& Out)
{
	Super::NativeOnDragDetected(In, InEvent, Out);
	st_Selected->Icon->SetVisibility(ESlateVisibility::Hidden);
}

bool UEquipUI::NativeOnDrop(const FGeometry& In, const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
	st_Selected->Icon->SetVisibility(ESlateVisibility::Hidden);

	// 자기 자신인 경우 슬롯이 같으면 교환됨
	if (st_SelectedParent == this || Cast<UInvenUI>(st_SelectedParent)) {
		Equip(GetHoveredSlot(InEvent), st_Selected);
	}

	return Super::NativeOnDrop(In, InEvent, InOperation);
}

void UEquipUI::NativeOnDragCancelled(const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
	st_Selected->Icon->SetVisibility(ESlateVisibility::Hidden);
	return Super::NativeOnDragCancelled(InEvent, InOperation);
}

void UEquipUI::Equip(Slot* This, Slot* From)
{
	UGenericItem *ThisItem  = Cast<UGenericItem>(This->Info),
				 *OtherItem = Cast<UGenericItem>(From->Info);

	// 캐스팅 실패시 From 이상한 데이터
	if (OtherItem == nullptr && From->Info != nullptr) {
		check(false);
	}

	EItemSlotTypeIdx Index = Indexer[This];
	
	// 빈 슬롯이 아니면 == Swap인 경우
	if (OtherItem) {
		// 같은 슬롯인지 검사 || 아이템 직업 유형 검사
		if (Index != OtherItem->Data.ItemSlotType || 
			!(static_cast<uint8>(m_Parent->ClassType) & OtherItem->Data.UsableClassTypes)) {
			// 메세지 출력
			UUIManager::Instance(this)->SetMessageText(_T("착용할 수 없는 아이템입니다."));
			return;
		}
	}

	// 각각 빈 슬롯이 아닌 경우
	if (ThisItem != nullptr)  ThisItem->OnUnequipment(m_Parent); // 탈착
	if (OtherItem != nullptr) OtherItem->OnEquipment(m_Parent);  // 장착

	// 교체
	Swap(This, From);
	return;
}

void UEquipUI::Equip(EItemSlotTypeIdx Index, Slot* From)
{
	int nIndex = static_cast<int>(Index) - 1;
	if (nIndex < 0 || nIndex >= m_Slots.Num()) {
		ensure(false);
		return;
	}
	Equip(&m_Slots[nIndex], From);
}

void UEquipUI::UpdatePlayerStatus()
{
	FString String = FString::Printf(
		_T("이름: %s\n레벨: %d\n공격력: %f"),
		*m_Parent->RowName.ToString(),
		 m_Parent->Level,
		 m_Parent->Current.Attack
	);
	PlayerStatus->SetText(FText::FromString(String));
}
