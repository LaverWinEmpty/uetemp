// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Manager/PlayerManager.h>
#include <LWE_WOW/Generic/GenericPlayer.h>
#include <LWE_WOW/Manager/LoadingManager.h>

#include <LWE_WOW/Common/Util.h>

// #include "PaperSpriteComponent.h"

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"

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
        PrimaryActorTick.bCanEverTick = true;

        static ConstructorHelpers::FObjectFinder<UMaterial> Material(
            _T("/Game/Resource/Material/BPM_Portal.BPM_Portal"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(
            _T("/Engine/EditorMeshes/EditorPlane.EditorPlane"));

        // 트리거 박스 생성
        TriggerBox = CreateDefaultSubobject<UBoxComponent>(_T("TriggerBox"));
        RootComponent = TriggerBox;

        // 트리거 박스 크기 설정
        TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        TriggerBox->SetBoxExtent({ 200, 200, 200 });

        // 콜리전 설정
        TriggerBox->SetCollisionObjectType(ECC_ACTOR_FINDER);
        TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
        TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        
        // 메시 컴포넌트 생성 및 설정
        MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(_T("MeshComponent"));
        MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

        // 트랜스폼
        MeshComp->SetRelativeScale3D({ 1.55, 1.55, 1.55 }); // 200 200 사이즈 기준
        MeshComp->SetRelativeLocation({ 200, 0, 0 });       // 맨 앞으로
        
        // 메쉬, 매테리얼
        MeshComp->SetStaticMesh(Mesh.Object);
        MeshComp->SetMaterial(0, Material.Object);

        // 동적
        MID = MID->Create(MeshComp->GetMaterial(0), this);
        MeshComp->SetMaterial(0, MID);
    }

public:
    virtual void BeginPlay() override {
        Super::BeginPlay();

        // 트리거박스 바인딩
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGate::OnOverlapBegin);

        // 텍스쳐 로딩
        UTexture2D* Texture = Cast<UTexture2D>(
            StaticLoadObject(
                UTexture2D::StaticClass(), nullptr,
                _T("/Game/Resource/Texture/Img_Portal.Img_Portal")
            )
        );

        if (Texture) 
            MID->SetTextureParameterValue(FName(_T("Texture")), Texture);
        else ensure(false);
    }

public:
    virtual void Tick(float DeltaTime) {
        Super::Tick(DeltaTime);
    
        m_T += DeltaTime;
        
        if (m_T >= 0.05) {
            MID->SetVectorParameterValue(FName(_T("UV")), m_UV);

            m_U += 1;
            if (m_U >= 4)
                m_U = 0;
            m_UV.X = m_U;
            m_T = 0;
        }
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
                // UGameplayStatics::OpenLevel(GetWorld(), LevelName);
                ULoadingManager::LoadLevel(this, LevelName);
            }
        }
    }

public:
    UPROPERTY(EditAnywhere)
    FName LevelName;

    UPROPERTY() UBoxComponent*            TriggerBox;
    UPROPERTY() UStaticMeshComponent*     MeshComp;
    UPROPERTY() UMaterialInstanceDynamic* MID;

private:
    FVector4 m_UV = FVector4{ 0, 0, 0.25, 1 };
    float    m_U  = 0;
    float    m_T  = 0;
};
