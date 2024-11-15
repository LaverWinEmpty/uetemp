// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericItem.h"

#include <LWE_WOW/Data/ItemData.h>
#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/UI/EquipUI.h>
#include <LWE_WOW/Manager/UIManager.h>

void UGenericItem::SetData(const FItemData& InData)
{
	Data = InData;
}

void UGenericItem::OnEquipment(AGenericCharacter* InParent)
{
	if (InParent->GetMesh()->DoesSocketExist(Data.SocketName)) {
		Mesh = Data.CreateMeshComponent(InParent);
		check(Mesh); // 생성 실패
	}

	// 임시
	InParent->Source.Attack += Data.Increment.Attack;
	InParent->UpdateStatus();
	PostEquipment();
 }


void UGenericItem::OnUnequipment(AGenericCharacter* InParent)
{
	Mesh->DestroyComponent();

	// 임시
	InParent->Source.Attack -= Data.Increment.Attack;
	InParent->UpdateStatus();
	PostEquipment();
}

void UGenericItem::PostEquipment()
{
	UEquipUI* UI = Cast<UEquipUI>(UUIManager::Instance(this)->Widgets[UUIManager::UI_EQUIPMENT]);
	UI->UpdatePlayerStatus();
}
