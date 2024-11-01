// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"

UPlayerManager* UPlayerManager::Instance(UObject* InWorldContextObject)
{
    if (void* Ptr = UGameplayStatics::GetGameInstance(InWorldContextObject->GetWorld())) {
        return static_cast<UGameInstance*>(Ptr)->GetSubsystem<UPlayerManager>();
    }
    return nullptr;
}