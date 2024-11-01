// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericMode.h"

#include <LWE_WOW/Setting/Default.h>
#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Manager/UIManager.h>

void AGenericMode::StartPlay()
{
	Super::StartPlay();

	// 한 방향의 각도를 0 ~ 179도 사이로 제한합니다
	// 0 이하, 360 이상은 보정합니다.
	if (CameraRangeHorizontal <  0)   CameraRangeHorizontal = 0;   // fixed
	if (CameraRangeHorizontal >= 360) CameraRangeHorizontal = 360; // unlimit
	else {
		CameraRangeHorizontal *= 0.5f;
		if (CameraRangeHorizontal > 179.9) {
			CameraRangeHorizontal = 179.9; // limit
		}
	}

	// 한 방향의 각도를 0 ~ 89도 사이로 제한합니다
	if (CameraRangeVertical < 0) CameraRangeVertical = 0; // fixed
	else {
		CameraRangeVertical *= 0.5f;
		if (CameraRangeVertical > 89.9) {
			CameraRangeVertical = 89.9; // limit
		}
	}

	if (ZoomMin < 0) ZoomMin = 0;

	// Set Global
	CDefault::MaxAngleH = CameraRangeHorizontal;
	CDefault::MaxAngleV = CameraRangeVertical;
	CDefault::ZoomMax   = ZoomMax;
	CDefault::ZoomMin   = ZoomMin;
}
