// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

bool CCamera::Setup(ACharacter* InParent)
{
	if (CUtil::Test(m_Parent = InParent, _T("GenericCamera::Parent"))) {
		// SpringArmgComponent를 가져옵니다
		m_ArmComponent = m_Parent->GetComponentByClass<USpringArmComponent>();
		// 없으면 생성합니다
		if (!m_ArmComponent) {
			m_ArmComponent = NewObject<USpringArmComponent>(m_Parent, USpringArmComponent::StaticClass(), _T("Arm"));
		}

		// CameraComponent를 찾습니다
		if (CUtil::Test(m_ArmComponent, _T("GenericCamera::Arm"))) {
			TArray<USceneComponent*> ChildComponents;
			m_ArmComponent->GetChildrenComponents(true, ChildComponents);

			for (USceneComponent* Child : ChildComponents) {
				if (Child->IsA<UCameraComponent>()) {
					m_CameraComponent = Cast<UCameraComponent>(Child);
					break;
				}
			}

			// 없으면 생성합니다
			if (!m_CameraComponent) {
				m_CameraComponent = NewObject<UCameraComponent>(m_ArmComponent, UCameraComponent::StaticClass(), _T("Camera"));
			}
		}
	}

	// 카메라를 최종적으로 만들기 때문에 카메라 존재 여부만 판단합니다
	if (CUtil::Test(m_CameraComponent, _T("GenericCamera::Camera"), CUtil::ON_DISPLAY)) {
		// 하이라키 설정
		m_ArmComponent->AttachToComponent(m_Parent->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		m_CameraComponent->AttachToComponent(m_ArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// 오브젝트 등록
		m_ArmComponent->RegisterComponent();
		m_CameraComponent->RegisterComponent();

		Initialzie();
		return true;
	}

	ensure(false);
	return false;
}

void CCamera::Initialzie()
{
	m_ArmComponent->TargetArmLength = m_Zoom;
	m_ArmComponent->ProbeChannel = ECC_Visibility;
	SetArmHeight(CDefault::DefVal::CAMERA_HEIGHT);
}

USpringArmComponent* CCamera::GetCameraArmComponent()
{
	return m_ArmComponent;
}

void CCamera::SetArmHeight(float In)
{
	FVector Vector = m_ArmComponent->GetRelativeLocation();
	Vector.Z = In;
	m_ArmComponent->SetRelativeLocation(Vector);
}

void CCamera::Zoom(float InUnit)
{
	m_Zoom += InUnit;
	if      (m_Zoom >= CDefault::ZOOM.MAX) m_Zoom = CDefault::ZOOM.MAX;
	else if (m_Zoom <= CDefault::ZOOM.MIN) m_Zoom = CDefault::ZOOM.MIN;
	m_ArmComponent->TargetArmLength = m_Zoom;
}

void CCamera::Rotate(const FVector2D& In)
{
	FRotator Rotator = m_ArmComponent->GetRelativeRotation();

	FVector2D Vector = In;
	//if (!FMath::IsNearlyEqual(Vector.Length(), 1)) {
	//	Vector.Normalize();
	//}
	//Vector *= (Speed * CUtil::GetDeltaTime(m_Parent, false));

	// 회전
	double Y = Rotator.Pitch + Vector.Y;
	double X = Rotator.Yaw   + Vector.X;

	// 좌우 무제한
	if (CDefault::MAX_CAMERA_ANGLE.X == 360) {
		if (X >= 360) X -= 360;
		if (X <= 0  ) X += 360;
	}

	// 좌우 제한
	else {
		if      (X >=  CDefault::MAX_CAMERA_ANGLE.X) X =  CDefault::MAX_CAMERA_ANGLE.X;
		else if (X <= -CDefault::MAX_CAMERA_ANGLE.X) X = -CDefault::MAX_CAMERA_ANGLE.X;
	}

	// 상하 제한
	if      (Y >=  CDefault::MAX_CAMERA_ANGLE.Y) Y =  CDefault::MAX_CAMERA_ANGLE.Y;
	else if (Y <= -CDefault::MAX_CAMERA_ANGLE.Y) Y = -CDefault::MAX_CAMERA_ANGLE.Y;

	// 적용
	Rotator.Yaw   = X;
	Rotator.Pitch = Y;

	m_ArmComponent->SetRelativeRotation(Rotator);
}
