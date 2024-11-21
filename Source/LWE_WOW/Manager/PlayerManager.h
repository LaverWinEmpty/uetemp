// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/CharacterInfo.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/UI/SlotInfo.h>
#include <LWE_WOW/UI/SlotUI.h>
#include <LWE_WOW/Class/Buff.h>

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/Pawn.h"
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
	void Save(AGenericCharacter* In);
	void Load(AGenericCharacter* Out);

private:
	void SetUISlotInfo();

public:
	FName PlayerNameOfDatatableRow;

public:
	FCharacterInfo Status;

	float HP, MP, SP;

public:
	// 상태도 저장해야 하나
	// AActor 기반으로 시전자 / 피격자 정보를 저장하는데
	// 레벨을 이동할 경우 시전자 피격자 정보가 없어질 가능성이 있어
	// 현재는 레벨 이동 시에는 초기화 되게끔 구현
	// Effect 자체가 UObject가 아니나 UObject를 상속하게 하는 방식
	// == 스킬과 같은 방식으로 구현 필요
	// TMap<FName, TSharedPtr<CBuff::Applied>> Applied;

public:
	// 슬롯 정보 저장용
	UPROPERTY()
	TArray<USlot*> Slots;
	
	// UI 저장용
	UPROPERTY()
	TArray<USlotUI*> UI;

public:
	bool IsSaved = false;
	
	bool IsGotItem  = false; // 아이템 로딩 완료 플래그
	bool IsGotSkill = false; // 스킬 로딩 완료 플래그
};