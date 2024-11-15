// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Data/ItemData.h>

#include "CoreMinimal.h"
#include "LWE_WOW/Generic/GenericCharacter.h"
#include "GenericPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API AGenericPlayer : public AGenericCharacter
{
	GENERATED_BODY()

public:
	ETargetType GetType() const;

public:
	virtual void Dead() override;
	
public:
	virtual void BeginPlay() override;

public:
	virtual void View(const FVector&) override;

public:
	virtual void Initialize() override;

public:
	void SetSkillSlot(EActionID InID, UGenericSkill* InSkill);
};
