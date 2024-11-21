// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Manager/UIManager.h>

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

#include "CoreMinimal.h"
#include "LevelUtils.h"
#include "MoviePlayer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoadingManager.generated.h"

class AGenericPlayer;

class UProgressBar;

/**
 *
 */
UCLASS()
class LWE_WOW_API ULoadingManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Out) override {
		Super::Initialize(Out);
		m_Bar        = nullptr;
		m_IsTickable = true;
	}

public:
	TStatId GetStatId() const override {
		return GetStatID();
	}

public:
	bool IsTickable() const override {
		return m_IsTickable;
	}

public:
	virtual void Tick(float DeltaTime) override {
		if (!m_Bar) return;
		check(m_Bar->IsA<UProgressBar>());

		// 진행률 계산
		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
		int32 TotalActors = AllActors.Num(); // 총 액터 개수
		// 여기서 뭐 하려 했는데 gg

		m_Bar->SetPercent((m_Progress += DeltaTime));

		if (m_Progress >= (TARGET_PROGRESS * 0.9)) {
			m_Bar      = nullptr;
			m_Progress = 0;
		}
	}

public:
	ULoadingManager() {
		static ConstructorHelpers::FClassFinder<UUserWidget> Finder[] = {
			{ _T("/Game/UI/UI_Loading.UI_Loading_C") },
		};
	
		int Size = sizeof(Finder) / sizeof(*Finder);
		m_Src.Init(nullptr, Size);
		for (int i = 0; i < Size; ++i) {
			m_Src[i] = Finder[i].Class;
		}
		m_Bar = nullptr;
	}

public:
	static ULoadingManager* Instance(UObject* InWorldContextObject) {
		if (UWorld* World = InWorldContextObject->GetWorld()) {
			if (UGameInstance* GI = UGameplayStatics::GetGameInstance(World)) {
				if (ULoadingManager* Self = GI->GetSubsystem<ULoadingManager>()) {
					return Self;
				}
			}
		}
		return nullptr;
	}

public:
	static void LoadLevel(UObject* InWorldContextObject, FName InLevelName, int InIndex = 0) {
		ULoadingManager* This = Instance(InWorldContextObject);

		UUserWidget* UI = CreateWidget<UUserWidget>(This->GetWorld(), This->m_Src[InIndex]);

		FLoadingScreenAttributes LoadingScreenAttributes;

		LoadingScreenAttributes.WidgetLoadingScreen               = UI->TakeWidget();
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime   = TARGET_PROGRESS; // 임시 값
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreenAttributes.bAllowEngineTick                  = true;

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);

		This->m_Bar = Cast<UProgressBar>(UUIManager::GetRoot(UI)->GetChildAt(0));
	
		UGameplayStatics::OpenLevel(This->GetWorld(), InLevelName);
	}

private:
	TArray<TSubclassOf<UUserWidget>> m_Src;

	UPROPERTY()
	UProgressBar* m_Bar = nullptr;

	float m_Progress;
	static constexpr float TARGET_PROGRESS = 3;

private:
	bool m_IsTickable = false;
};
