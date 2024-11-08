// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss.h"

#include <LWE_WOW/Data/CharacterData.h>
#include <LWE_WOW/Data/SkillData.h>
#include <LWE_WOW/Generic/GenericBossAI.h>

ABoss::ABoss()
{
	// AI 컨트롤러를 세팅합니다.
	AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AGenericBossAI::StaticClass();

	// 컨트롤러로 조작할 것이기 때문에 회전값을 줍니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
}

ABoss::~ABoss()
{
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Param;

	// 보스의 영역을 설정합니다.
	// 컴포넌트가 없어서 우선 0 0 0 으로 초기화 합니다
	AActor* BossAreaFixer =
		GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), { 0, 0, 0 }, { 0, 0, 0 }, Param);

	// 영역을 생성합니다.
	BossArea = NewObject<USphereComponent>(BossAreaFixer, _T("Area"));

	// 세팅
	BossArea->SetSphereRadius(BossAreaRadius);
	BossArea->SetCollisionObjectType(ECC_ACTOR_SEARCH);
	BossArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	BossArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BossArea->RegisterComponent();

	// 루트컴포넌트로
	BossAreaFixer->SetRootComponent(BossArea);

	// 사이즈, 스케일, 회전 값 보정합니다.
	BossAreaFixer->SetActorLocation(GetActorLocation() + AreaLocation);
	BossAreaFixer->SetActorScale3D(GetActorScale3D() * AreaScale);
	BossAreaFixer->SetActorRotation(AreaRotation);

	m_Distance = FindDistance;
}

void ABoss::Tick(float DeltaTime)
{
	// 행동은 AI컨트롤러로 제어합니다.
	AGenericCharacter::Tick(DeltaTime);
}

void ABoss::MoveTick()
{
	// AMob::MoveTick 은 랜덤 지역 자동 이동이기 때문에 사용하지 않습니다.
	AGenericCharacter::MoveTick();
}

void ABoss::Dead()
{
	Super::Dead();
	GetController()->UnPossess();
}
