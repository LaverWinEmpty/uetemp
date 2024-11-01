// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Setting/Default.h>
#include <LWE_WOW/Common/Util.h>

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"



/**
 * 
 */
struct LWE_WOW_API CCamera
{
public:
	CCamera();
	virtual ~CCamera();

public:
	bool Setup(ACharacter* In);

protected:
	void Initialzie();

public:
	USpringArmComponent* GetCameraArmComponent();

public:
	void SetArmHeight(float In);

public:
	void Zoom(float InUnit);

public:
	void Rotate(const FVector2D& In);

private:
	ACharacter* m_Parent;

private:
	TObjectPtr<USpringArmComponent> m_ArmComponent;
	TObjectPtr<UCameraComponent>    m_CameraComponent;

private:
	float m_Zoom = CDefault::DefVal::CAMERA_ZOOM;

private:
	bool m_IsCameraMovable          = true;  // 카메라가 시점 변경 가능 플래그
	bool m_IsCameraFollowMouse      = true;  // 카메라가 마우스를 따라갑니다
	bool m_IsMoveHorizontalByCamera = true;  // 객체가 카메라 시점을 따라갑니다
	bool m_IsMoveVrticalByCamera    = false; // 객체가 카메라 상하 시점을 따라갑니다
};
