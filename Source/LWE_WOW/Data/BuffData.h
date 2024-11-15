// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Status/BuffInfo.h>

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "BuffData.generated.h"


class AGenericCharacter;

// 이펙트 플래그
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EBuffFlag : uint8 {
	NONE,
	INFINITE       = 1 << 0 UMETA(DisplayName = "InfiniteDuration"),
	SKIP_BEGIN     = 1 << 1 UMETA(DisplayName = "Skip On Begin"),
	SKIP_TICK      = 1 << 2 UMETA(DisplayName = "Skip On Tick"),
	SKIP_END       = 1 << 3 UMETA(DisplayName = "Skip On End"),
	EALRY_TICK     = 1 << 4 UMETA(DisplayName = "First Tick With On Begin"),
	IGNORE_STAT    = 1 << 5 UMETA(DisplayName = "Ignore Resistance Stat"),
	KEEP_WHEN_DEAD = 1 << 6 UMETA(DisplayName = "Effect Keep When Died"),
};
ENUM_CLASS_FLAGS(EBuffFlag);

// 상태이상, 버프, 디버프 등과 관련된 데이터입니다.
UCLASS(Blueprintable)
class LWE_WOW_API UBuffData : public UDataAsset
{
	GENERATED_BODY()

public:
	// 효과 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FName Name = "";
	
	// 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString Descript = "";

	// 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	UTexture2D* Image;

	// 스킬 자체 최대 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	int MaxLevel = 1;

	// 버프 정보입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FBuffInfo Default;

	// 레벨 당 증가 정보입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FBuffInfo Coefficient; 

	// 옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information", Meta = (Bitmask, BitmaskEnum = EBuffFlag))
	uint8 Option;

public:
	int                      AdjustLevel(int InLevel) const;
	virtual FBuffInfo::State Initialize()             const;
	virtual FBuffInfo        Calculate(int InLevel)   const;

public:
	virtual void OnBegin(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const;
	virtual void OnTick(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const;
	virtual void OnEnd(float InPower, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const;
};