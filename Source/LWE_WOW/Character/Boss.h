// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Character/Mob.h>

#include "CoreMinimal.h"
#include "AIController.h"
#include "Boss.generated.h"

UCLASS()
class LWE_WOW_API ABoss : public AMob // public AGenericCharacter
{
	GENERATED_BODY()

public:
	ABoss();
	~ABoss();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float) override;

public:
	virtual void MoveTick() override;

public:
	virtual void Dead() override;

public:
	USphereComponent* BossArea;

public:
	// 보스 영역 반지름입니다.
	UPROPERTY(EditAnywhere, Category = "AI")
	float BossAreaRadius;

public:
	// 위치 보정에 사용됩니다.
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector AreaLocation = { 0, 0, 0 };
	
	// 스케일 보정에 사용됩니다.
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector AreaScale = { 1, 1, 1 };	

	// 각도 보정에 사용됩니다.
	UPROPERTY(EditAnywhere, Category = "AI")
	FRotator AreaRotation = { 0, 0, 0};
};
