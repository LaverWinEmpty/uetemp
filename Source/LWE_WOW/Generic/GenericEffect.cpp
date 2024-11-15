// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericEffect.h"
#include "Components/MeshComponent.h"   
#include "Components/SphereComponent.h"
#include "Materials/MaterialInterface.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericSkill.h>


AGenericEffect::AGenericEffect()
{
	PrimaryActorTick.bCanEverTick = true;
    m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(_T("Mesh"));
    m_Area = CreateDefaultSubobject<USphereComponent>(_T("Area"));

    RootComponent = CreateDefaultSubobject<USceneComponent>(_T("Root"));
    
    m_Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    m_Area->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    // 플레이어 관련 정보만 켭니다
    m_Area->SetCollisionObjectType(ECC_ACTOR_FINDER);
    m_Area->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_Area->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AGenericEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericEffect::Tick(float DeltaTime)
{
    // LOG(1, "%s", *GetActorRotation().ToString());
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
    if (m_Status->Info.Speed == 0) {
        // 그리고 각도를 캐릭터 정면으로 고정합니다
        SetActorRotation(m_Parent->GetActorForwardVector().Rotation());
        m_bExecute = true;
    }

    // 소환형 스킬인 경우에도 바로 시전합니다.
    else if (m_Status->Data->Option & static_cast<uint8>(ESkillFlag::IS_SUMMON)) {
        SetActorRotation(m_Parent->GetActorForwardVector().Rotation());
        m_bExecute = true;
        SetActorLocation(Seeker.End); // 해당 위치로 이동해버립니다.
    }

    else {
        // 논타겟팅 스킬이 아니라면
        // 해당 "캐릭터"의 위치를 계속해서 쫓아갑니다
        if ((m_Status->Data->Option & static_cast<uint8>(ESkillFlag::IS_GUIDED))) {
            if (m_Target != nullptr) {
                Seeker.SetTarget(m_Target->GetActorLocation());
                Seeker.View(); // 해당 캐릭터를 바라봅니다.
            }
        }

        // 이동 함수입니다.
        Seeker.OnTick();
        // 논타겟 투척 스킬이라면
        if (m_Status->Info.Speed != 0 &&(m_Status->Data->Option & static_cast<uint8>(ESkillFlag::IS_NON_TARGET))) {
            SkillMethod(&USkillData::OnTick); // Tick이 여기에서 실행됩니다.
        }

        // 회전 속도값 적용 시
        if (m_Status->Data->RotOpt & static_cast<uint8>(ESkillRotateOption::ROTATE_ON_TICK_SHOOT)) {
            // 방향을 사이즈로 변환 시켜 쿼터니언 기반으로 회전시킵니다.
            FQuat QuatRotation = FQuat(m_Status->Data->RotOnTick.GetSafeNormal(), m_Status->Data->RotOnTick.Size() * DeltaTime);
            m_Mesh->AddRelativeRotation(QuatRotation);
        }

        // 도달했는지 검사합니다.
        if (Seeker.IsAtTarget()) {
            SetActorLocation(Seeker.End); // 해당 위치로 보정합니다.
            // 회전 최종값 활성화 돼있다면 해당 회전 값으로 보정합니다.
            if (m_Status->Data->RotOpt & static_cast<uint8>(ESkillRotateOption::ROTATE_END)) {
                m_Mesh->SetRelativeRotation(m_Status->Data->RotEndTick); // 보정
            }
            SkillMethod(&USkillData::Execute); // 실행
            m_bExecute = true;
        }
    }

    // Duration 없으면 바로 종료합니다.
    if (m_bExecute && !m_Status->Info.Duration) {
        SkillMethod(&USkillData::Final); // 종료
        Destroy();
    }
}

void AGenericEffect::Enable(float DeltaTime)
{
    if (!m_Status || !m_Status->IsA<UGenericSkill>() || !IsValid(m_Status) || !m_Status->Data) {
        check(false);
    }

    // 회전 속도값 적용 시
    if (m_Status->Data->RotOpt & static_cast<uint8>(ESkillRotateOption::ROTATE_ON_TICK_ENABLE)) {
        // 방향을 사이즈로 변환 시켜 쿼터니언 기반으로 회전시킵니다.
        FQuat QuatRotation = FQuat(m_Status->Data->RotOnTick.GetSafeNormal(), m_Status->Data->RotOnTick.Size() * DeltaTime);
        m_Mesh->AddRelativeRotation(QuatRotation);
    }

    // 추적 옵션이 켜져있다면
    if (m_Status->Data->Option & static_cast<uint8>(ESkillFlag::IS_FOLLOW)) {
        if (m_Target != nullptr) {
            Seeker.SetTarget(m_Target->GetActorLocation());
            Seeker.View(); // 해당 캐릭터를 바라봅니다.
            Seeker.OnTick();

            // 그냥 위치 고정
            SetActorLocation(m_Target->GetActorLocation());
        }
    }

    m_Interval += DeltaTime;
    // 틱 있음
    if (m_Status->Info.Interval) {
        if (m_Interval >= m_Status->Info.Interval) {
            m_Duration += m_Status->Info.Interval;
            m_Interval = 0;
            SkillMethod(&USkillData::OnTick); // 실행
        }
    }

    // 틱 없으면 바로
    if ((m_Duration + m_Interval) > m_Status->Info.Duration) {
        SkillMethod(&USkillData::Final); // 실행
        Destroy();
    }
}

void AGenericEffect::SkillMethod(FnSkillMethod MethodName)
{
    if (m_Status->Data->Option & static_cast<uint8>(ESkillFlag::IS_NON_TARGET)) {
        TArray<AActor*> Overlapped;
        m_Area->GetOverlappingActors(Overlapped);
        for (AActor* Actor : Overlapped) {
            if (AGenericCharacter* Character = Cast<AGenericCharacter>(Actor)) {
                // 죽은 경우
                if (Character->IsDead && !(m_Status->Data->Option & static_cast<uint8>(ESkillFlag::IS_CAN_DEAD))) 
                    continue;
                // Call
                if (m_Status->Data->Target & static_cast<uint8>(m_Parent->GetRelation(Character)))
                    (m_Status->Data->*MethodName)(m_Status, m_Parent, Character, this);

            } // end if
        } // end for
    } // end if

    // 타겟팅은 시전 중 검증됩니다.
    else (m_Status->Data->*MethodName)(m_Status, m_Parent, m_Target, this);
}

AGenericEffect* AGenericEffect::Spawn(UGenericSkill* InSkill, AGenericCharacter* InParent, AGenericCharacter* InTarget)
{
    // 위치 지정 없을 시 액터 앞에 소환됩니다.
    return Spawn(InSkill, InParent, InTarget, InParent->GetActorLocation() + InParent->GetActorForwardVector() * 100);
}

AGenericEffect* AGenericEffect::Spawn(UGenericSkill* InSkill, AGenericCharacter* InParent, AGenericCharacter* InTarget, const FVector& InPos)
{
    AGenericEffect* Spawned = Spawn(InSkill->Data, InParent, InPos);
    if (Spawned) {
        Spawned->m_Status = InSkill;
        Spawned->Initialize(InSkill->Data, InSkill->Level, *InSkill->GetName(), InParent, InTarget, InPos);
    }
    return Spawned;
}

AGenericEffect* AGenericEffect::Spawn(USkillData* InSkillData, int InLevel, const TCHAR* const InName, 
    AGenericCharacter* InParent, AGenericCharacter* InTarget, const FVector& InPos)
{
    AGenericEffect* Spawned = Spawn(InSkillData, InParent, InPos);
    if (Spawned) {
        Spawned->m_Status = NewObject<UGenericSkill>(Spawned, InName);
        Spawned->m_Status->SetData(InSkillData, InLevel);
        Spawned->Initialize(InSkillData, InLevel, InName, InParent, InTarget, InPos);
    }
    return Spawned;
}

AGenericEffect* AGenericEffect::Spawn(USkillData* InSkillData, AGenericCharacter* InParent, const FVector& InLocation)
{
    UWorld* World = InParent->GetWorld();
    if (!World) {
        return nullptr;
    }

    FActorSpawnParameters Param;
    Param.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FRotator Rotation = InParent->GetActorRotation();
    FVector  Location = InLocation; // 지정된 위치에 소환됩니다.

    AGenericEffect* Spawned =
        World->SpawnActor<AGenericEffect>(StaticClass(), Location, Rotation, Param);

    Spawned->Owner = Spawned;
    Spawned->m_Mesh->SetStaticMesh(InSkillData->Mesh);
    if (!Spawned->m_Mesh) {
        return nullptr;
    }
    Spawned->m_Mesh->AddLocalRotation(InSkillData->Rotation);

    // 적용할 머테리얼 수 / 필요한 머테리얼 수 비교
    int Loop = FMath::Min(InSkillData->Material.Num(), Spawned->m_Mesh->GetNumMaterials());
    for (int i = 0; i < Loop; ++i) {
        UMaterialInterface* MaterialToSet = InSkillData->Material[i];
        if (MaterialToSet) {
            Spawned->m_Mesh->SetMaterial(i, MaterialToSet);
        }
    }

    return Spawned;
}

void AGenericEffect::Initialize(USkillData* InData, int InLevel, const TCHAR* const InName,
    AGenericCharacter* InParent, AGenericCharacter* InTarget, const FVector& InLocation)
{
    check(m_Status);

    m_Parent = InParent;
    m_Target = InTarget;

    FSkillInfo* InfoRef = &m_Status->Info;

    m_Mesh->SetCollisionProfileName(TEXT("NoCollision"));
    m_Mesh->SetWorldScale3D({ InfoRef->Scale, InfoRef->Scale, InfoRef->Scale });
    m_Mesh->SetRelativeRotation(InData->Rotation);

    // Non Target이라면
    if ((InData->Option & static_cast<uint8>(ESkillFlag::IS_NON_TARGET))) {
        // 현재 위치에서 전방 벡터 사거리 까지
        Seeker.Setting(InLocation, InLocation + GetActorForwardVector() * InfoRef->Range);

        // 공격 영역 세팅
        m_Area->SetWorldScale3D({ InfoRef->Area, InfoRef->Area, InfoRef->Area });
#if WITH_EDITOR
        m_Area->bHiddenInGame = false;
#endif
    }

        // Parent의 Target이 자동으로 세팅됩니다.
    else Seeker.Setting(InParent);

    Seeker.SetSpeed(InfoRef->Speed);
    Seeker.View(); // 해당 방향 보도록 회전
}
