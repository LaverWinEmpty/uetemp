#pragma once

#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Status/CharacterInfo.h>

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ItemData.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct LWE_WOW_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
	
	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FName Name = "Unknown";

	// 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FString Descript = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	EItemSlotTypeIdx ItemSlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information", Meta = (Bitmask, BitmaskEnum = EClassCode))
	uint8 UsableClassTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	EItemMeshType ItemMeshType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (EditCondition = "ItemMeshType==EItemMeshType::WEAPON", EditConditionHides))
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (EditCondition = "ItemMeshType==EItemMeshType::ARMOR", EditConditionHides))
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (EditCondition = "ItemMeshType!=EItemMeshType::CONSUM", EditConditionHides))
	FVector MeshLocationAdjust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (EditCondition = "ItemMeshType!=EItemMeshType::CONSUM", EditConditionHides))
	FRotator MeshRotationAdjust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (EditCondition = "ItemMeshType!=EItemMeshType::CONSUM", EditConditionHides))
	FVector Scale = { 1, 1, 1 };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FName SocketName;

	// 아이템 레벨 제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	int Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FCharacterInfo Increment;

public:
	UPrimitiveComponent* CreateMeshComponent(ACharacter* InParent);
};