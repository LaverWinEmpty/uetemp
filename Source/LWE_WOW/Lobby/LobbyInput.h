// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Manager/PlayerManager.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/Generic/GenericCharacter.h>

#include "CoreMinimal.h"
#include "LWE_WOW/Generic/GenericInput.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "LobbyInput.generated.h"


/**
 * 
 */
UCLASS()
class LWE_WOW_API ALobbyInput : public AGenericInput
{
	GENERATED_BODY()
	
	ALobbyInput() : AGenericInput() {
		static ConstructorHelpers::FClassFinder<UUserWidget> Finder{ _T("/Game/UI/UI_Lobby.UI_Lobby_C") };
		m_Src = Finder.Class;
	}

	virtual void Initialize(UEnhancedInputComponent* EIC) {
		EIC->BindAction(m_IA[IA_MOUSE_L], ETriggerEvent::Started, this, &AGenericInput::OnInput<bool>);
		Mapping(IA_MOUSE_L, EKeys::LeftMouseButton);

		// 해당 맵 전용 UI

		m_UI = CreateWidget<UUserWidget>(GetWorld(), m_Src);
		m_UI->AddToViewport();

		UPanelWidget *Root, *Panel;
		UButton* Btn;

		Root = UUIManager::GetRoot(m_UI);

		Panel = Cast<UPanelWidget>(Root->GetChildAt(0));
		Btn   = Cast<UButton>(Panel->GetChildAt(0));
		Btn->OnClicked.AddDynamic(this, &ALobbyInput::OkButton);
		
		m_MsgBox = Cast<UWidget>(Panel);

		Panel = Cast<UPanelWidget>(Root->GetChildAt(1));
		Btn   = Cast<UButton>(Panel->GetChildAt(0));
		Btn->OnClicked.AddDynamic(this, &ALobbyInput::StartButton);

		HideMessageBox();

		m_Clicked = nullptr; // 초기화

		m_Camera->SetArmHeight(0);
	}


public:
	virtual bool PostInput(bool, EActionID, ETriggerEvent) {
		// 클릭만 있음

		// 플래그: 메세지 박스 활성화됨	
		if (m_bShowMessageBox) {
			++DebugMode;
			if (DebugMode == 10) {
				if (m_Clicked) {
					UPlayerManager::Instance(this)->PlayerNameOfDatatableRow = m_Clicked->RowName;
				}
				else UPlayerManager::Instance(this)->PlayerNameOfDatatableRow = _T("");
				UGameplayStatics::OpenLevel(GetWorld(), _T("TestMap"));
			}
			return true;
		}
		else DebugMode = 0;

		// 좌클릭만 처리해서 이동 시 반환 false
		auto Hit = OnMouseClick();
		if (Hit) {

			m_Clicked = Cast<AGenericCharacter>(Hit->GetActor());
			if (m_Clicked == m_Player) {
				m_Clicked = nullptr;
			}
			if (m_Clicked) {
				static const FName LIST[] = {
					"Attack_0",
					"Attack_1",
					"Attack_2",
					"Attack_3",
					"Attack_4",
					"Attack_5",
					"Attack_6",
					"Attack_7",
				};

				// 몽타주 재생 중이 아닐 때만
				if (!m_Clicked->AnimationBase->IsAnyMontagePlaying()) {
					// 50% 확률로 캐스팅모션
					if (FMath::Rand() & 1) {
						m_Clicked->AnimationBase->Montage_Play(m_Clicked->CastingMotion);
						return true;
					}

					// 몽타주 재생
					m_Clicked->AnimationBase->Montage_Play(m_Clicked->AttackMotion);
					// 랜덤
					m_Clicked->AnimationBase->Montage_JumpToSection(LIST[FMath::Rand() & 0b111], m_Clicked->AttackMotion);
				}
			}
		}

		return true;
	}

public:
	void HideMessageBox() {
		if (!m_bShowMessageBox) {
			return;
		}
		m_bShowMessageBox = false;
		m_MsgBox->SetVisibility(ESlateVisibility::Hidden);
	}

	void ShowMessageBox() {
		if (m_bShowMessageBox) {
			return;
		}
		m_bShowMessageBox = true;
		m_MsgBox->SetVisibility(ESlateVisibility::Visible);
	}

	UFUNCTION()
	void OkButton() {
		HideMessageBox();
	}

	UFUNCTION()
	void StartButton() {
		if (!m_Clicked) {
			ShowMessageBox();
			return;
		}
		
		UPlayerManager::Instance(this)->PlayerNameOfDatatableRow = m_Clicked->RowName;
		UGameplayStatics::OpenLevel(GetWorld(), _T("FieldMap"));
	}
	
private:
	AGenericCharacter*       m_Clicked;
	UUserWidget*             m_UI;
	UWidget*                 m_MsgBox;
	TSubclassOf<UUserWidget> m_Src;
	bool                     m_bShowMessageBox = true;

private:
	int DebugMode = 0;

};
