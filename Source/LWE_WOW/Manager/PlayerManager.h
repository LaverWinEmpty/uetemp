// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/CharacterInfo.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/UI/InvenUI.h>
#include <LWE_WOW/UI/EquipUI.h>
#include <LWE_WOW/UI/SkillUI.h>
#include <LWE_WOW/UI/QuickSlotUI.h>

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerManager.generated.h"

class AGenericPlayer;

/**
 * 
 */
UCLASS()
class LWE_WOW_API UPlayerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UPlayerManager* Instance(UObject* InWorldContextObject);

public:
	FName PlayerNameOfDatatableRow;

public:
	// 여기 작성중

	// 캐릭터를 생성
	AGenericPlayer* CreatePlayer();

	void SaveOnMoveLevel(AGenericPlayer* In);
	void LoadOnMoveLevel(AGenericPlayer*& Out);


private:
	template<typename T>
	void OnSave(UUIManager::EUIList Index) {
		 T* UI = Cast<T>(UUIManager::Instance(this)->Widgets[Index]);
		 for (int i = 0; i < UI->m_Slots.Num(); ++i) {
			 Slots.Add(TStrongObjectPtr<UObject>(UI->m_Slots[i]));
		 }
	}

private:
	FCharacterInfo PlayerStatus;             // 현재 상태 저장용
	TArray<TStrongObjectPtr<UObject>> Slots; // 슬롯 저장용
};