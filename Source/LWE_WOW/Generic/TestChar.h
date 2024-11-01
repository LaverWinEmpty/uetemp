// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Generic/GenericCharacter.h>

#include "CoreMinimal.h"
#include "TestChar.generated.h"

/**
 * 
 */
UCLASS()
class LWE_WOW_API ATestChar : public AGenericCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float MoveValue = 0.05;

	FVector2d Area;
	FVector3d Moved = { 0, 0, 0 };

	void BeginPlay() {
		Super::BeginPlay();
		Area = { GetActorLocation().X, GetActorLocation().Y };

		float X = FMath::RandRange(Area.X - 100, Area.X + 100);
		float Y = FMath::RandRange(Area.Y - 100, Area.Y + 100);
		StartTargetMove({ X, Y, 0 });

		Ref = RefCnt++;
	}

	void Tick(float D) override {
		// 이동 처리
		Super::Tick(D);
		if (IsDead)
			return;

		float Distance = (Moved - GetActorLocation()).SizeSquared();

		// 뭔가 막혔으면
		if (Distance <= MoveValue) {
			IsTargetMove = false;
		}

		Time += D;
		if (Time >= 3) {
			Time = 0;

			// 위치 재설정
			float X = FMath::RandRange(Area.X - 300, Area.X + 300);
			float Y = FMath::RandRange(Area.Y - 300, Area.Y + 300);
			StartTargetMove({ X, Y, 0 });
			Moved = { 0, 0, 0 }; // 현재 프레임과 비교하게 되어 초기화
		}

		// else 처리
		else Moved = GetActorLocation();
	}

	float Time  = 0;
	int   Count = 10;
	int   Flag  = 0;


	int Ref;
	inline static int RefCnt = 0;
};
