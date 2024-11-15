// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/SkillInfo.h>
#include <LWE_WOW/Interface/Slotable.h>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GenericItem.generated.h"

class AGenericCharacter;
struct FItemData;

/**
 * 
 */
UCLASS()
class LWE_WOW_API UGenericItem : public UObject
{
	GENERATED_BODY()

public:
	void SetData(const FItemData&);

public:
	void OnEquipment(AGenericCharacter*);
	void OnUnequipment(AGenericCharacter*);

private:
	void PostEquipment();

public:
	FItemData Data;

private:
	UPROPERTY()
	UPrimitiveComponent* Mesh = nullptr;
};
