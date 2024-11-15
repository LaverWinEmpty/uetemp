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

class UAnimMontage;
class AGenericCharacter;
class AGenericEffect;
class UGenericSkill;

// 이펙트 플래그
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESkillFlag : uint8 {
	NONE,
	// 정면에서만 사용할 수 있습니다.
	LIMIT_VIEW    = 1 << 0 UMETA(DisplayName = "Limit View"),
	// On/Off 스킬입니다.
	TOGGLE        = 1 << 1 UMETA(DisplayName = "Toggle"),
	// 발사하는 동안 (비주얼적으로) 유도됩니다.
	IS_GUIDED     = 1 << 2 UMETA(DisplayName = "Guided"),
	// 발사하고난 후 (비주얼적으로) 달라붙습니다.
	IS_FOLLOW     = 1 << 3 UMETA(DisplayName = "Follow"),
	// 논타겟 스킬입니다. 발사체인 경우 Shoot 중 Tick이 발생하게 됩니다.
	IS_NON_TARGET = 1 << 4 UMETA(DisplayName = "Non-target"),
	// 소환형 스킬입니다.
	IS_SUMMON     = 1 << 5 UMETA(DisplayName = "Summon"),
	// 자기 자신에게 사용됩니다.
	IS_USE_SELF   = 1 << 6 UMETA(DisplayName = "Can Self"),
	// 죽은 상대에게 사용 가능합니다.
	IS_CAN_DEAD   = 1 << 7 UMETA(DisplayName = "Can Dead"),
};
ENUM_CLASS_FLAGS(ESkillFlag);

// 이펙트 플래그 회전 관련
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESkillRotateOption : uint8 {
	NONE,
	// 발사 도중 회전하는지 여부입니다.
	ROTATE_ON_TICK_SHOOT  = 1 << 0 UMETA(DisplayName = "Rotate on shoot in Tick"),
	// 발사가 끝나고 유지하는 동안 회전하는지 여부입니다.
	ROTATE_ON_TICK_ENABLE = 1 << 1 UMETA(DisplayName = "Rotate on enable in Tick"),
	// 발사가 끝나면 목표 회전 값으로 보정할 것인지 여부입니다.
	ROTATE_END = 1 << 2 UMETA(DisplayName = "Rotate adjust in Enable"),
};
ENUM_CLASS_FLAGS(ESkillRotateOption);

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
		OnTick:  스킬 시작 ~ 종료 사이 틱마다 발동하는 효과입니다.

		단 Non-Target 스킬에 속도 값이 존재하면 투척 중 피격되는 스킬로 취급됩니다.
		이 경우 OnTick -> Execute -> OnTick -> Final 순으로 실행됩니다.
		Duration을 0으로 두면 2번째 OnTick은 무시할 수 있습니다.

		스킬(Effect)이 Spawn되면 목표를 향해 이동하거나, 설치하는 등의 효과가 발생합니다.
		그 이후 Execute가 호출됩니다.

		Parameter
		UGenericSkill     In       스킬 정보입니다.
		AGenericCharacter InCaster 시전자입니다.
		AGenericCharacter InTarget 타겟입니다.
		AGenericEffect    InActor  현재 스킬로 생성된 이펙트 액터입니다.
	*/

	virtual void Execute(UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const;
	virtual void Final  (UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const;
	virtual void OnTick (UGenericSkill* In, AGenericCharacter* InCaster, AGenericCharacter* InTarget, AGenericEffect* InActor) const;

public:
	// 사용할 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Mesh;

	// 사용할 머테리얼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UMaterial*> Material;

	// 메시 보정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")

	FRotator Rotation;

	// 파생 스킬들, 필요한 경우에만 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	TArray<TSubclassOf<USkillData>> Next;

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

	// 틱마다 회전하고 싶을 때 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FVector RotOnTick = { 0, 0, 0 };

	// 틱 끝나고 최종 회전값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FRotator RotEndTick = { 0, 0, 0 };
	
	// 회전 값 옵션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information", Meta = (Bitmask, BitmaskEnum = ESkillRotateOption))
	uint8 RotOpt = static_cast<uint8>(ESkillRotateOption::NONE);

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

protected:
	void ApplyBuff(AGenericCharacter* InCaster, AGenericCharacter* InTarget, int Level) const;
};