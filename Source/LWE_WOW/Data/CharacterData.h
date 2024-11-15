#pragma once

#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Status/CharacterInfo.h>

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "CharacterData.generated.h"

class USkillData;
class UAnimInstance;

USTRUCT(BlueprintType, Blueprintable)
struct LWE_WOW_API FCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FVector MeshLocationAdjust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FRotator MeshRotationAdjust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FVector Scale = { 1, 1, 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimationBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimationAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimationCasting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimationDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AnimationHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<UMaterial*> Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKill")
	TArray<TSubclassOf<USkillData>> UseableSkillList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	int DefaultLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FCharacterInfo Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FCharacterInfo Coefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	EClassCode ClassType;
};