// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreen.h"
#include "Components/Image.h"

bool ULoadingScreen::Initialize()
{
	return Super::Initialize();
}

void ULoadingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CheckResources(LoadingBackgroundImage);
	CheckResources(LoadingAnimationImage);
}

void ULoadingScreen::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void ULoadingScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULoadingScreen::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULoadingScreen::CheckResources(UImage* InImage)
{
	if (UTexture2D* BrushTexture = Cast<UTexture2D>(InImage->GetBrush().GetResourceObject()))
	{
#if WITH_EDITOR
		if (BrushTexture->LODGroup != TextureGroup::TEXTUREGROUP_UI)
		{
			ensureMsgf(false, TEXT("%s Texture->LODGroup != TEXTUREGROUP_UI"), *InImage->GetName());
		}
#else
		BrushTexture->LODGroup = TextureGroup::TEXTUREGROUP_UI;
		BrushTexture->UpdateResource();
#endif
	}
}
