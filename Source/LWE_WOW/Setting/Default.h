#pragma once

#include "CoreMinimal.h"

/**
 *  나중에 Json 등으로 교체 필요
 */
struct LWE_WOW_API CDefault
{
private:
	CDefault()  = delete;
	~CDefault() = delete;

public:
	// default key setting
	struct DefKey {
	private:
		DefKey() = delete;

	public:
		inline static const FKey JUMP     = EKeys::SpaceBar;
		inline static const FKey MOVE_F   = EKeys::W;
		inline static const FKey MOVE_B   = EKeys::S;
		inline static const FKey MOVE_L   = EKeys::A;
		inline static const FKey MOVE_R   = EKeys::D;
		inline static const FKey MOUSE    = EKeys::Mouse2D;
		inline static const FKey ZOON_IN  = EKeys::MouseScrollUp;
		inline static const FKey ZOOM_OUT = EKeys::MouseScrollDown;
	};

	// default etc values
	struct DefVal {
		inline static const float CAMERA_ZOOM   = 200;
		inline static const float CAMERA_HEIGHT = 60;
	};

private:
	// Set on gamemode
	friend class AGenericMode;

	inline static float MaxAngleH;
	inline static float MaxAngleV;
	inline static float ZoomMin;
	inline static float ZoomMax;

public:
	// for get
	inline static const struct {
		const float& X;	
		const float& Y;
	} MAX_CAMERA_ANGLE = { MaxAngleH, MaxAngleV };

public:
	// for get
	inline static const struct {
		const float& MIN;
		const float& MAX;
	} ZOOM = { ZoomMin, ZoomMax };
};
