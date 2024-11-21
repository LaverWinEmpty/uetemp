#pragma once

#include <LWE_WOW/Generic/GenericUI.h>
#include <LWE_WOW/Common/Constants.h>

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/TextBlock.h"
#include "UIManager.generated.h"

class UGenericUI;
class UQuickSlotUI;
class UProgressBar;

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
        UI_INVENTORY,
        UI_EQUIPMENT,
        UI_SKILL,

        // Insert...
        
        UI_QUICKSLOT_0,
        UI_QUICKSLOT_1,

        UI_END,
        UI_BEGIN = 0,
        UI_QUICKSLOT_BEGIN = UI_QUICKSLOT_0,
        UI_QUICKSLOT_END   = UI_END,
    };

public:
    bool Setup();
    void Cleanup();
    void Reset();

public:
    // 현재 UI ShowState에 따라 On/Off 됩니다.
    void SetUI();

public:
    UUIManager();
    
public:
    void SetMessageText(const TCHAR* const);

public:
    static UPanelWidget* GetRoot(UUserWidget*);

public:
    static UUIManager* Instance(UObject* InWorldContextObject);

public:
    void SetTargetInfo(AActor* In);

public:
    UQuickSlotUI* GetSkillSlot(EActionID);

public:
    void Show(EUIList, bool);
    void Toggle(EUIList);

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
    TArray<TSubclassOf<UGenericUI>> Resource; // 외부에서 가져올 변수

public:
    UGenericUI* Widgets[UI_END];

private:
	UWidget*      m_Dialog_Exit;
    UTextBlock*   m_MsgBox;
    UImage*       m_DeadOverride;
    UProgressBar* m_LoadingBar;

public:
    bool UIShowState[UI_END] = {
        true,  // Main
        false, // Inven
        false, // Euqip
        false, // Skill
        true,  // Qucik 0
        true,  // Qucik 1
    };

public:
    bool IsSetup : 1 = false;

private:
	bool m_bShowExitDialog : 1 = true; // 로그아웃 메세지 박스
};

/*
    inline void Set(int InWidgetIndex, int InResourceIndex) {
        if (Resource[InResourceIndex] == nullptr)
            return;
        Widgets[InWidgetIndex] = CreateWidget<UUserWidget>(GetWorld(), Resource[InResourceIndex]);
        Widgets[InWidgetIndex]->AddToViewport();
    }
*/