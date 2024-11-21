// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Manager/UIManager.h>

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
	virtual void Initialize() override {
		//if (UUIManager* Instance = UUIManager::Instance(this)) {
		//	// 로비는 모든 UI를 끕니다.
		//	for (int i = Instance->UI_BEGIN; i < Instance->UI_END; ++i) {
		//		Instance->Widgets[i]->SetVisibility(ESlateVisibility::Hidden);
		//	}
		//}
	}

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
