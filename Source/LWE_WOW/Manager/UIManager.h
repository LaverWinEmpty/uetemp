#pragma once

#include "Components/TextBlock.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class UUserWidget;
class UGenericIcon;

/**
 * 
 */
UCLASS()
class LWE_WOW_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    enum EUIList {
        UI_MAIN,
        UI_SKILL_SLOT_1,
        UI_END,
        UI_BEGIN = 0,
    };

public:
    void Setup();
    void Cleanup();

public:
    void Reset();

public:
    UUIManager();
    
public:
    void SetMessageText(const TCHAR* const);

public:
    static UPanelWidget* GetRoot(UUserWidget*);

public:
    static UUIManager* Instance(UObject* InWorldContextObject);

public:
    inline void SetTargetInfo(AActor* In);

public:
	void HideExitDialogBox();

    void ShowMessageBox();

	UFUNCTION()
    void ExitButton();

	UFUNCTION()
    void NoButton();

	UFUNCTION()
    void YesButton();

public:
    void OnDead();
    void OnResurrect();

public:
    FTimerHandle MessageBoxStarter;
    FTimerHandle MessageBoxUpdater;

private:
    TArray<TSubclassOf<UUserWidget>> Resource; // 외부에서 가져올 변수

public:
    UUserWidget* Widgets[UI_END];

private:
	UWidget*    m_Dialog_Exit;
    UTextBlock* m_MsgBox;
    UImage*     m_DeadOverride;

private:
    bool m_IsSetup         = false;
	bool m_bShowExitDialog = true; // 로그아웃 메세지 박스
};

/*
    inline void Set(int InWidgetIndex, int InResourceIndex) {
        if (Resource[InResourceIndex] == nullptr)
            return;
        Widgets[InWidgetIndex] = CreateWidget<UUserWidget>(GetWorld(), Resource[InResourceIndex]);
        Widgets[InWidgetIndex]->AddToViewport();
    }
*/