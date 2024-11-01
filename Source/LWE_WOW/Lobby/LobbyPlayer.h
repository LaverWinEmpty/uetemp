// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LWE_WOW/Generic/GenericCharacter.h"
#include "LobbyPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API ALobbyPlayer : public AGenericCharacter
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override {
		ACharacter::BeginPlay();
		GetCharacterMovement()->DisableMovement();
		// Not Work
	}

	virtual void Tick(float) override {
		// Not Work
	}
};
