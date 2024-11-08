// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Manager/PlayerManager.h>
#include <LWE_WOW/Generic/GenericPlayer.h>

#include "Kismet/GameplayStatics.h"

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "Gate.generated.h"


/**
 * 
 */
UCLASS()
class LWE_WOW_API AGate : public AActor
{
    GENERATED_BODY()

public:
    AGate() {
        // 트리거 박스 생성
        TriggerBox = CreateDefaultSubobject<UBoxComponent>(_T("TriggerBox"));
        RootComponent = TriggerBox;

        // 트리거 박스 크기 설정
        TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        TriggerBox->SetBoxExtent({ 200, 200, 200 });

        // 콜리곤 설정
        TriggerBox->SetCollisionObjectType(ECC_ACTOR_SEARCH);
        TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
        TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    }

public:
    virtual void BeginPlay() override {
        Super::BeginPlay();

        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGate::OnOverlapBegin);

        SetActorScale3D(Scale);
    }

public:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                        AActor*              OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32                OtherBodyIndex,
                        bool                 bFromSweep,
                        const FHitResult&    SweepResult) {

        if (OtherActor && (OtherActor != this)) {

            // 플레이어만 레벨 전환 가능
            if (AGenericPlayer* Player = Cast<AGenericPlayer>(OtherActor)) {

                // 플레이어 정보를 가져와야 하지만
                // 임시로 데이터 테이블에서 가져옵니다.
                UPlayerManager::Instance(this)->PlayerNameOfDatatableRow = Player->RowName;

                // 지정된 레벨로 이동합니다.
                UGameplayStatics::OpenLevel(GetWorld(), LevelName);
            }

        }
    }

public:
    UPROPERTY(EditAnywhere)
    FVector Scale;

    UPROPERTY(EditAnywhere)
    FName LevelName;

    UBoxComponent* TriggerBox;
};
