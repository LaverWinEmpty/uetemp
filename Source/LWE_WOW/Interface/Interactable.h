// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Common/Constants.h>

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI) class UInteractable : public UInterface { GENERATED_BODY() };

class LWE_WOW_API IInteractable
{
	GENERATED_BODY()

public:
	virtual ETargetType   GetType()                   const;
	virtual ETeamType     GetTeam()                   const;
	virtual ERelationType GetRelation(IInteractable*) const;
	virtual void          OnTargetting();
	
public:
	static void SetNeutral(ETeamType& Out);
	static void SetEnemy(ETeamType& Out);

public:
	bool IsPlayer() const;
};

