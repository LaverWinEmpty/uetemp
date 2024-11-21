// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreen.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API ULoadingScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void CheckResources(UImage* InImage);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* LoadingBackgroundImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* LoadingAnimationImage = nullptr;
};
