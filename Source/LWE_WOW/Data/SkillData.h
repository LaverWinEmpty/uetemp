	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Status/SkillInfo.h>
#include <LWE_WOW/Data/BuffData.h>

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Components/MeshComponent.h"

#include "SkillData.generated.h"

class AGenericCharacter;
class UAnimMontage;

// 이펙트 플래그
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESkillFlag : uint8 {
	NONE,
	IGNORE_STAT  = 1 << 0 UMETA(DisplayName = "Ignore resistance stat"),
	LIMIT_VIEW   = 1 << 1 UMETA(DisplayName = "Can use when target is visible only"),
	TOGGLE       = 1 << 2 UMETA(DisplayName = "On/Off skill"),
	IS_SUMMON    = 1 << 3 UMETA(DisplayName = "Summon or trap skill"),
	IS_DODGEABLE = 1 << 4 UMETA(DisplayName = "Target can dodge"),
	IS_GUIDED    = 1 << 5 UMETA(DisplayName = "Follow target (visual)"),
	IS_SELF      = 1 << 6 UMETA(DisplayName = "If no target, use to self"),
	IS_TO_DEAD   = 1 << 7 UMETA(DisplayName = "Use to dead character"),
};
ENUM_CLASS_FLAGS(ESkillFlag);

UCLASS(Blueprintable)
class LWE_WOW_API USkillData : public UDataAsset
{
	GENERATED_BODY()

	
public:
	USkillData();

public:
	/*
		Execute: 스킬 시작 직후 발생하는 효과입니다.
		Final:   스킬 종료 직전 발생하는 효과입니다.
		OnTikcl: 스킬 시작 ~ 종료 사이 틱마다 발동하는 효과입니다.

		스킬(Effect)이 Spawn되면 목표를 향해 이동하거나, 설치하는 등의 효과가 발생합니다.
		그 이후 Execute가 호출됩니다.
	*/

	virtual void Execute(const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const;
	virtual void Final  (const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const;
	virtual void OnTick (const FSkillInfo& In, AGenericCharacter* InCaster, AGenericCharacter* InTarget) const;

public:
	// 사용할 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Mesh;

	// 사용할 머테리얼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UMaterial*> Material;
		
	// 스킬 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FName Name = "Unknown";

	// 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString Descript = "";

	// 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	UTexture2D* Image;

	// 전용 캐스팅 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	UAnimMontage* CastingMotion;

	// 전용 실행 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	UAnimMontage* ExecuteMotion;

	// 해당 스킬 사용 시 타겟에게 적용 될 이펙트 종류입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	TArray<TSubclassOf<UBuffData>> Apply;

	// 고정 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FSkillInfo Default;

	// 계수: 퍼센트 단위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FSkillInfo Coefficient;

	// 스킬을 배울 수 있는 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	int LearnLevel = 1;

	// 스킬 자체 최대 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	int MaxLevel = 1;

	// 배울 수 있는 직업
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information", Meta = (Bitmask, BitmaskEnum = EClassCode))
	uint8 Job = static_cast<uint8>(EClassCode::NONE);

	// 사용 가능한 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information", Meta = (Bitmask, BitmaskEnum = EWeaponCode))
	uint8 Weapon = static_cast<uint8>(EWeaponCode::NONE);

	// 가능한 타겟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information", Meta = (Bitmask, BitmaskEnum = ERelationType))
	uint8 Target = static_cast<uint8>(ERelationType::NONE);

	// 기타 옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information", Meta = (Bitmask, BitmaskEnum = ESkillFlag))
	uint8 Option;

public:
	virtual FSkillInfo Calculate(int InLevel)                 const;
	int                AdjustLevel(int InLevel)               const;
	void               ApplyBuff(AGenericCharacter* InTarget) const;
};