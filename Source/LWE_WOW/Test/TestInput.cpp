// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInput.h"

void ATestInput::SetupInputComponent()
{
	Super::SetupInputComponent();

	this->bShowMouseCursor = true;
}

void ATestInput::PostMouseLeftClick(const TOptional<FHitResult>& InHit)
{	
	Super::PostMouseLeftClick(InHit);
}

void ATestInput::PostMouseRightClick(const TOptional<FHitResult>& InHit)
{
	Super::PostMouseRightClick(InHit);
}
