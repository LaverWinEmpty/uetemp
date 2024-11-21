// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/SkillInfo.h>
#include <LWE_WOW/UI/SlotInfo.h>
#include <LWE_WOW/Data/ItemData.h>

#include "CoreMinimal.h"
#include "GenericItem.generated.h"

class AGenericCharacter;
struct FItemData;

/**
 * 
 */
UCLASS()
class LWE_WOW_API UGenericItem : public USlot
{
	GENERATED_BODY()

public:
	void SetData(const FItemData&);

public:
	void OnEquipment(AGenericCharacter*);
	void OnUnequipment(AGenericCharacter*);

public:
	void CreateMesh(AGenericCharacter*);
	void DestroyMesh();

private:
	void PostEquipment();

public:
	FItemData Data;

private:
	UPROPERTY()
	UPrimitiveComponent* Mesh = nullptr;
};
