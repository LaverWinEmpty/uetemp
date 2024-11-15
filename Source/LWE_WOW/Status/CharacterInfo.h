// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Common/Constants.h>

#include "CoreMinimal.h"

#include "CharacterInfo.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct LWE_WOW_API FCharacterInfo
{
	GENERATED_BODY()

public:
	FCharacterInfo();
	~FCharacterInfo();

public:
	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float HP;            

	// 마나 또는 그에 준하는 자원
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float MP;

	// 기타 추가 자원
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float SP;

	// 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float STR;

	// 민?첩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float DEX;

	// 지능
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float INT;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Attack;
		
	// 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Defence;
		
	// 이동속도 관련
	UPROPERTY(BlueprintReadWrite, Category = "Information") 
	float Speed;

	// 공격속도 관련
	UPROPERTY(BlueprintReadWrite, Category = "Information") 
	float Delay;
		
	// 치명타확률 예시: 50% => 50
	UPROPERTY(BlueprintReadWrite, Category = "Information") 
	float CriticalHit;

	// 치명타배율 예시 50% 추가 => 150
	UPROPERTY(BlueprintReadWrite, Category = "Information")
	float CriticalMulit;

	// 부가 스탯 (와우: 유연)
	UPROPERTY(BlueprintReadWrite, Category = "Information")
	float Bonus;         

	// 부가 스탯 (와우: 특화)
	UPROPERTY(BlueprintReadWrite, Category = "Information")
	float Extra;         
};
