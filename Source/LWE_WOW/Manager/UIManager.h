#pragma once

#include <LWE_WOW/Generic/GenericUI.h>

#include "Components/TextBlock.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

class UGenericUI;
class UQuickSlotUI;

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
    void Setup();
    void Cleanup();

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
	UWidget*    m_Dialog_Exit;
    UTextBlock* m_MsgBox;
    UImage*     m_DeadOverride;

private:
    bool m_IsSetup         : 1 = false;
	bool m_bShowExitDialog : 1 = true; // 로그아웃 메세지 박스

    

    void* Old = nullptr;
};

/*
    inline void Set(int InWidgetIndex, int InResourceIndex) {
        if (Resource[InResourceIndex] == nullptr)
            return;
        Widgets[InWidgetIndex] = CreateWidget<UUserWidget>(GetWorld(), Resource[InResourceIndex]);
        Widgets[InWidgetIndex]->AddToViewport();
    }
*/