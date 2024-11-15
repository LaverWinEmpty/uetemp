#include "InvenUI.h"

#include <LWE_WOW/Common/Util.h>

#include <LWE_WOW/Generic/GenericItem.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/UI/EquipUI.h>

void UInvenUI::NativeConstruct()
{
	Super::NativeConstruct();

	//테스트 코드: 아이템 생성
	if (UDataTable* Table = LoadObject<UDataTable>(nullptr, _T("/Game/Data/Tables/Items.Items"))) {
		if (FItemData* Row = Table->FindRow<FItemData>(_T("Default"), _T("Player Weapon"))) {
			AddItem(*Row);
		}
		if (FItemData* Row = Table->FindRow<FItemData>(_T("Spear"), _T("Player Weapon"))) {
			AddItem(*Row);
		}
		if (FItemData* Row = Table->FindRow<FItemData>(_T("Axe_0"), _T("Player Weapon"))) {
			AddItem(*Row);
		}
		if (FItemData* Row = Table->FindRow<FItemData>(_T("Axe_1"), _T("Player Weapon"))) {
			AddItem(*Row);
		}
		if (FItemData* Row = Table->FindRow<FItemData>(_T("Hummer"), _T("Player Weapon"))) {
			AddItem(*Row);
		}
	}
}

FReply UInvenUI::NativeOnMouseButtonDown(const FGeometry& In, const FPointerEvent& InEvent)
{
	if (InEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		Slot* Temp = GetHoveredSlot(InEvent);
		if (!Temp) {
			return FReply::Unhandled();
		}

		if (UGenericItem* Item = Cast<UGenericItem>(Temp->Info)) {
			// 장착 가능하면
			if (Item->Data.ItemSlotType != EItemSlotTypeIdx::NONE) {
				// UI 가져와서
				if (UEquipUI* UI = Cast<UEquipUI>(UUIManager::Instance(this)->Widgets[UUIManager::UI_EQUIPMENT])) {

					// 슬롯 찾아서 알아서 장착 시도
					UI->Equip(Item->Data.ItemSlotType, Temp);
				}
				else check(false); // failed Cast
			}

			else {
				// 소모품 사용 나중에 구현
			}
		}
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(In, InEvent);
}

FReply UInvenUI::NativeOnMouseButtonUp(const FGeometry& In, const FPointerEvent& InEvent)
{
	return Super::NativeOnMouseButtonUp(In, InEvent);
}

void UInvenUI::NativeOnDragDetected(const FGeometry& In, const FPointerEvent& InEvent, UDragDropOperation*& Out)
{
	Super::NativeOnDragDetected(In, InEvent, Out);
	st_Selected->Icon->SetVisibility(ESlateVisibility::Hidden);
}

bool UInvenUI::NativeOnDrop(const FGeometry& In, const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
	st_Selected->Icon->SetVisibility(ESlateVisibility::Visible);

	// 창 내부에서 움직이는 것
	if (st_SelectedParent == this) {
		Swap(GetHoveredSlot(InEvent), st_Selected);
	}

	// 장비창에서 옮기는 거라면 장비 해제
	else if (UEquipUI* UI = Cast<UEquipUI>(st_SelectedParent)) {
		// 장비창 꽉 차있으면 무시
		if (Slot* Temp = GetEmptySlot()) {
			UI->Equip(st_Selected, Temp);
		}
	}

	return Super::NativeOnDrop(In, InEvent, InOperation);
}

void UInvenUI::NativeOnDragCancelled(const FDragDropEvent& InEvent, UDragDropOperation* InOperation)
{
	st_Selected->Icon->SetVisibility(ESlateVisibility::Visible);

	RemoveItem(st_Selected);
	Super::NativeOnDragCancelled(InEvent, InOperation);
}

auto UInvenUI::GetEmptySlot()->Slot*
{
	// 빈 자리 아무데나
	int Loop = m_Slots.Num();
	for (int i = 0; i < Loop; ++i) {
		if (m_Slots[i].Info == nullptr) {
			return &m_Slots[i];
		}
	}

	return nullptr;
}

void UInvenUI::AddItem(const FItemData& InData)
{
	UGenericItem* NewItem = NewObject<UGenericItem>(this);
	NewItem->SetData(InData);

	if (Slot * Temp = GetEmptySlot()) {
		Temp->Icon->SetBrushFromTexture(InData.Icon);
		Temp->Info = NewItem;
		return;
	}

	UUIManager::Instance(this)->SetMessageText(_T("가방이 가득 찼습니다."));
}

void UInvenUI::RemoveItem(Slot* In)
{
	In->Icon->SetBrushFromSoftTexture(EMPTY);
	In->Info->MarkAsGarbage();
}
