// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericEffect.h"
#include "Components/MeshComponent.h"   
#include "Materials/MaterialInterface.h"

#include <LWE_WOW/Generic/GenericCharacter.h>


AGenericEffect::AGenericEffect()
{
	PrimaryActorTick.bCanEverTick = true;
    m_Mesh        = CreateDefaultSubobject<UStaticMeshComponent>(_T("Mesh"));
    RootComponent = m_Mesh;
}

void AGenericEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (m_bExecute) {
        Enable(DeltaTime);
    }
    else Shoot(DeltaTime);
}

void AGenericEffect::PostActorCreated()
{
	Super::PostActorCreated();
}

void AGenericEffect::Shoot(float DeltaTime)
{
    // 이동할 수 없으면 바로 시전합니다.
    if (m_Info.Speed == 0) {
        m_bExecute = true;
    }

    else {
        // 유도 옵션이 켜진 경우
        // 해당 "캐릭터"의 위치를 계속해서 쫓아갑니다
        if (m_Data->Option & static_cast<int>(ESkillFlag::IS_GUIDED)) {
            Seeker.SetTarget(m_Target->GetActorLocation());
            Seeker.View(); // 해당 캐릭터를 바라봅니다.
        }

        // 이동 함수입니다.
        Seeker.OnTick();

        // 도달했는지 검사합니다.
        if (Seeker.IsAtTarget()) {
            SetActorLocation(Seeker.End); // 해당 위치로 보정합니다.

            // 소환형 스킬이 아닌데 회피 가능한 경우
            if ((m_Data->Option & static_cast<int>(ESkillFlag::IS_GUIDED)) &&
                !(m_Data->Option & static_cast<int>(ESkillFlag::IS_SUMMON))) {

                // 목표 위치와 타겟 위치를 제곱으로 계산한 다음 범위 제곱과 비교합니다.
                if ((Seeker.End - m_Target->GetActorLocation()).SquaredLength() <= (m_Info.Area * m_Info.Area)) {
                    m_Data->Execute(m_Info, m_Parent, m_Target); // 범위 안이라면 시전합니다.
                    m_bExecute = true;
                }

                else {
                    Destroy(); // 범위 밖이라면 취소합니다. (범위 안 객체 검사 구현 필요)
                }
            }
            else {
                m_Data->Execute(m_Info, m_Parent, m_Target); // 아니면 바로 시전합니다.
                m_bExecute = true;
            }
        }
    }

    // Duration 없으면 바로 종료합니다.
    if (m_bExecute && !m_Info.Duration) {
        m_Data->Final(m_Info, m_Parent, m_Target);
        Destroy();
    }
}

void AGenericEffect::Enable(float DeltaTime)
{
    m_Interval += DeltaTime;
    // 틱 있음
    if (m_Info.Interval) {
        if (m_Interval >= m_Info.Interval) {
            m_Duration += m_Info.Interval;
            m_Interval = 0;
            m_Data->OnTick(m_Info, m_Parent, m_Target); // 틱 호출
        }
    }

    if ((m_Duration + m_Interval) > m_Info.Duration) {
        m_Data->Final(m_Info, m_Parent, m_Target); // 라스트 호출
        Destroy();
    }
}

void AGenericEffect::Spawn(Skill* InSkill, AGenericCharacter* InParent, AGenericCharacter* InTarget)
{
    UWorld* World = InParent->GetWorld();
    if (!World) {
        return;
    }

    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; 

    FVector  Location = InParent->GetActorLocation() + (InParent->GetActorForwardVector() * 100); // 조금 앞에서 소환됩니다
    FRotator Rotation = InParent->GetActorRotation();

    AGenericEffect* Spawned =
        World->SpawnActor<AGenericEffect>(StaticClass(), Location, Rotation, Param);

    Spawned->Owner = Spawned;
    Spawned->m_Mesh->SetStaticMesh(InSkill->Data->Mesh);
    if (!Spawned->m_Mesh) {
        return;
    }

    // 적용할 머테리얼 수 / 필요한 머테리얼 수 비교
    int Loop = FMath::Min(InSkill->Data->Material.Num(), Spawned->m_Mesh->GetNumMaterials());
    for (int i = 0; i < Loop; ++i) {
        UMaterialInterface* MaterialToSet = InSkill->Data->Material[i];
        if (MaterialToSet) {
            Spawned->m_Mesh->SetMaterial(i, MaterialToSet);
        }
    }

    Spawned->m_Info   = InSkill->Info;
    Spawned->m_Data   = InSkill->Data;
    Spawned->m_Parent = InParent;
    Spawned->m_Target = InTarget;

    Spawned->m_Mesh->SetCollisionProfileName(TEXT("NoCollision"));
    Spawned->m_Mesh->SetWorldScale3D({ Spawned->m_Info.Scale, Spawned->m_Info.Scale, Spawned->m_Info.Scale });

    Spawned->Seeker.Setting(InParent);
    Spawned->Seeker.SetSpeed(Spawned->m_Info.Speed);
    Spawned->Seeker.View(); // 해당 방향 보도록 회전
}