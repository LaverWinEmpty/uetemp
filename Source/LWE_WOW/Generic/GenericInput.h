// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Class/Act.h>
#include <LWE_WOW/Setting/Default.h>
#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Class/Camera.h>

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#include "GenericInput.generated.h"

class AGenericCharacter;

/**
 * 
 */
UCLASS()
class LWE_WOW_API AGenericInput : public APlayerController
{
	GENERATED_BODY()

	static constexpr float PLAYER_MESH_HIDE_ZOOM = 50;

public:
	AGenericInput();
	~AGenericInput();

public:
	virtual void OnPossess(APawn*) override;
	virtual void SetupInputComponent() override;

private:
	template<typename Arg, typename... Args> void PostMapping(FEnhancedActionKeyMapping* Out) {
		if constexpr (TIsDerivedFrom<Arg, UInputModifier>::IsDerived) Out->Modifiers.Add(NewObject<Arg>());
		if constexpr (TIsDerivedFrom<Arg, UInputTrigger>::IsDerived)  Out->Triggers.Add(NewObject<Arg>());
		if constexpr (sizeof...(Args) > 0)
			PostMapping<Args...>(Out);
	}

public:
	template<typename Arg, typename... Args> void Mapping(EActionID InID, FKey InKey) {
		PostMapping<Arg, Args...>(&m_IMC->MapKey(m_IA[InID], InKey));
	}
	void Mapping(EActionID, FKey);
	void Unmapping(EActionID, FKey);

public:
	void CreateAction(EActionID);

private:
	void BindPushKey(UEnhancedInputComponent*, EActionID);

public:
	virtual void Initialize(UEnhancedInputComponent*);

public:
	virtual bool PostInput(bool,             EActionID, ETriggerEvent);
	virtual bool PostInput(float,            EActionID, ETriggerEvent);
	virtual bool PostInput(const FVector2d&, EActionID, ETriggerEvent);

public:
	template<typename T> void OnInput(const FInputActionInstance& InAction) {
		PostInput(InAction.GetValue().Get<T>(), m_Table[InAction.GetSourceAction()], InAction.GetTriggerEvent());
	}

public:
	TOptional<FHitResult> OnMouseClick();
	void                  OnMouseLeftClick(bool);
	void                  OnMouseRightClick(bool);

public:
	void PlayerAction(EActionID);

public:
	void SetTargetOfPlayer(IInteractable* InOther);
	void UnsetTargetOfPlayer();

public:
	// 마우스 왼쪽 버튼을 누르기 시작한 시점에만 호출됩니다.
	virtual void PostMouseLeftClick (const TOptional<FHitResult>&);
	// 마우스 오른쪽 버튼을 누르기 시작한 시점에만 호출됩니다.
	virtual void PostMouseRightClick(const TOptional<FHitResult>&);

private:
	EModFlag m_Mod = static_cast<EModFlag>(0); // Ctrl, etc...

public:
	const UInputAction* const GetInputAction(EActionID) const;
	EActionID                 GetInputActionID(const UInputAction* const) const;

public:
	void Look(const FVector2d& InValue, EModFlag InFlag);
	void LookFollow();
	void Jump();
	void Zoom(float InValue);
	void Move(const FVector2d&, bool);
	void MoveTarget(const FVector& InTarget);

public:
	USpringArmComponent* GetCameraArm();

protected:
	AGenericCharacter*                    m_Player;
	TObjectPtr<UInputMappingContext>      m_IMC;
	TArray<TObjectPtr<UInputAction>>      m_IA;
	TMap<UInputAction*, EActionID>        m_Table;
	TArray<TEnumAsByte<EObjectTypeQuery>> m_Trace;

protected:
	struct MouseClicekd {
		IInteractable* Target = nullptr; // 클릭 됐는지 검사용
		FVector2d      Screen = FVector2d { 0, 0 };
		FVector        World  = FVector   { 0, 0, 0 };
		bool           Flag   = false;
	};
	MouseClicekd m_Left, m_Right;

protected:
	struct {
		int X, Y;
		// 움직이는 상태인지 검사
		operator bool() { return X || Y; }
	} m_PlayerInputWay; // 입력 방향을 가르키는 벡터입니다.

protected:
	// 카메라 모듈입니다.
	TSharedPtr<CCamera> m_Camera;

protected:
	// 카메라 설정값입니다.
	UPROPERTY(EditAnywhere, Category = "Camera"); float CameraSpeed = 100;
	UPROPERTY(EditAnywhere, Category = "Camera"); float CameraZoomUnit = 25;
};
