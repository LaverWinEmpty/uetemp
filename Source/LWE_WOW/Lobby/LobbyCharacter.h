// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Interface/Interactable.h>

#include "CoreMinimal.h"
#include "LWE_WOW/Generic/GenericCharacter.h"
#include "LobbyCharacter.generated.h"

UCLASS()
class LWE_WOW_API ALobbyCharacter : public AGenericCharacter
{
	GENERATED_BODY()

public:
	ALobbyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
