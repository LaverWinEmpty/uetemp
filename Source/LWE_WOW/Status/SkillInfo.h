#pragma once

#include "SkillInfo.generated.h"

// 스킬 시전 정보
USTRUCT(BlueprintType)
struct LWE_WOW_API FSkillInfo
{
	GENERATED_BODY()

	FSkillInfo();
	~FSkillInfo();

	// 데미지 등 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Power;
	
	// 소비 마나 등 능력치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Energe;
	
	// 쿨타임 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Cooldown;
	
	// 차지 스킬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Charge;
	
	// 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Range;
	
	// 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Area;
	
	// 타겟 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Target;
	
	// 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Duration;

	// 틱 발생 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Interval;

	// 투사체 속도, 0의 경우 즉발
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Speed;

	// 스킬 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Scale;
};