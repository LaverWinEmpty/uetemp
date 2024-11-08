// Fill out your copyright notice in the Description page of Project Settings.


#include "Mob.h"

#include <LWE_WOW/Generic/GenericSkill.h>

AMob::AMob()
{
	Finder = CreateDefaultSubobject<USphereComponent>(_T("Finder"));
	Finder->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// 플레이어 관련 정보만 켭니다
	Finder->SetCollisionObjectType(ECC_ACTOR_SEARCH);
	Finder->SetCollisionResponseToAllChannels(ECR_Ignore);
	Finder->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AMob::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	Finder->SetSphereRadius(FindDistance); // SetRelativeScale3D({ FindDistnace, FindDistnace, FindDistnace });

	// rescale
	FVector InvScale = GetActorScale();
	InvScale.X = 1 / InvScale.X;
	InvScale.Y = 1 / InvScale.Y;
	InvScale.Z = 1 / InvScale.Z;
	Finder->SetRelativeScale3D(InvScale);

	m_Movement = Cast<UCharacterMovementComponent>(GetMovementComponent());

	SkillCount = SkillList.Num();
}

void AMob::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	m_Time += DeltaTime;
	if (IsDead)
		return;

	// 전투 상태면 전투
	if (IsBattle) {
		BattleTick();
		return;
	}

	// 복귀중엔 타겟팅하지 않음
	if (!IsReturn) {
		TArray<AActor*> Overlapped;
		Finder->GetOverlappingActors(Overlapped);
		for (AActor* Actor : Overlapped) {
			// 다른 캐릭터라면
			if (AGenericCharacter* Character = Cast<AGenericCharacter>(Actor)) {
				if (Character->IsDead) {
					continue; // 죽었으면 무시
				}
				if (Character->IsIgnore) {
					continue; // 타겟팅할 수 없음
				}
				Target.Setting(Character);
				if (Target.GetRelation() != ERelationType::HARM) {
					continue; // 적이 아님
				}
				StartBattle(); // 전투 시작
			}
		}
	}
}

void AMob::MoveTick()
 {
	Super::MoveTick();

	float MovedDist = (Moved - GetActorLocation()).SquaredLength();

	// 뭔가 막혔으면
	if (MovedDist <= MoveValue ||
		// OR 복귀 중인데 이동 범위 절반까지 이동했다면
		(IsReturn && CUtil::IsIn(this, InitialLocation, WaitAreaRange * 0.5))) {
			
		IsTargetMove = false;           // 이동 종료
		m_Movement->MaxWalkSpeed = 600; // 속도 복구
		IsReturn = false;               // 복귀 종료
	}

	// 전투중이 아닐 때 랜덤 이동 처리
	if (!IsBattle) {
		if (m_Time >= m_MoveUpdateTime) {
			m_Time -= m_MoveUpdateTime;

			if (WaitAreaRange != 0) {
				// 위치 재설정
				float X = FMath::RandRange(InitialLocation.X - WaitAreaRange, InitialLocation.X + WaitAreaRange);
				float Y = FMath::RandRange(InitialLocation.Y - WaitAreaRange, InitialLocation.Y + WaitAreaRange);
				StartTargetMove({ X, Y, 0 });
				m_MoveUpdateTime = FMath::RandRange(UPDATE_TIME_MIN, UPDATE_TIME_MAX);
			}

			// 이동 여부를 현재 프레임과 비교하기 위해 임의 값으로 초기화
			Moved.X += 1;
		}
	}

	// 움직이는 상태라면
	if (IsMoving()) {
		// 현재 위치 저장
		Moved = GetActorLocation();
	}
}


void AMob::OnHit(AGenericCharacter* InHiter)
{
	Super::OnHit(InHiter);
	if (!Target) {
		Target.Setting(InHiter); // set
		IsBattle = true;
	}
}

void AMob::StartBattle()
{
	SelectedSkill = SelectSkill();
	if (!SelectedSkill) {
		return;
	}
		
	Tracking(); // 추적
	IsBattle = true;

	m_Time = m_BattleUpdateTime; // 바로 업데이트 처리
}

void AMob::BattleTick()
{
	// 전투 종료 조건
	if (AGenericCharacter* Character = Target.Get<AGenericCharacter>()) {
		if (Character->IsDead || Character->IsIgnore) {
			Target.Unsetting();
			IsBattle = false;
		}
	}

	// 틱마다 지정된 스킬 범위 안인지 검사
	Tracking();

	// 타겟이 있으면
	if (Target) {
		// 죽었으면 종료
		if (Target.Get<AGenericCharacter>()->IsDead) {
			IsBattle = false;
		}

		// 업데이트 시간에 스킬 세팅
		if (m_Time >= m_BattleUpdateTime) {
			m_Time -= m_BattleUpdateTime;

			// 스킬 시전중 아닌 경우
			if (!Act) {
				SelectedSkill = SelectSkill();
				if (!SelectedSkill) {
					return;
				}

				// 고른 스킬의 사거리를 추적 거리로 세팅합니다.
				m_Distance = SelectedSkill->Info.Range;
			}

			// 추적 상태 아니면, 즉 사거리 내 적이 있다면
			if (!IsTracking) {
				Act.Start(SelectedSkill); // 시전
			}
		}
	}

	// 타겟 없으면 종료
	else {
		IsBattle = false;
		return;
	}
}

void AMob::Tracking()
{
	// 범위 밖으로 이동했으면 || 타겟이 있으나 너무 멀어졌으면
	if (!CUtil::IsIn(this, InitialLocation, FollowAreaRange) ||
		(Target && !CUtil::IsIn(this, Target.Get<AActor>(), FindDistance))) {
		StartTargetMove(InitialLocation);
		IsBattle = false;
		IsReturn = true;
		Target.Unsetting();
		m_Movement->MaxWalkSpeed = 1500; // 빠르게 복귀
		m_Time = 0; // 업데이트 대기
		return;
	}

	// 타겟 잃음 즉 전투 종료면 종료
	if (!Target) {
		return;
	}

	FVector Vector = GetActorLocation() - Target.Get<AGenericCharacter>()->GetActorLocation();
	float   Dist   = Vector.SquaredLength();
	float   Range  = m_Distance * m_Distance;

	// 사거리 검사합니다.
	if (Dist >= Range) {
		Vector.Normalize();                           // 범위 안으로 이동하기 위해 방향 지정 
		Vector *= FMath::Sqrt(Dist - Range);          // 실제 차이나는 거리 값
		StartTargetMove(GetActorLocation() - Vector); // 해당 방향까지 이동

		IsTracking = true;
		return;
	}

	// 회전 합니다.
	else {
		// 방향벡터 2D로
		FVector Dir = CUtil::RemoveZ(Target.Get<AGenericCharacter>()->GetActorLocation() - GetActorLocation());
		Dir.Normalize();
		SetActorRotation(Dir.Rotation()); // 회전
		IsTargetMove = false; // 이동 정지
	}

	IsTracking = false;
}

UGenericSkill* AMob::SelectSkill()
{
	// 랜덤으로 스킬을 골라 사용합니다.
	int Index = FMath::RandRange(0, SkillCount - 1);

	UGenericSkill* Ret = nullptr;

	// 쿨타임이라면 다음 스킬을 찾습니다.
	for (int i = Index; i < SkillCount; ++i) {
		UGenericSkill* Temp = SkillList[SkillTable[i]];
		if (Temp->Cooldown == 0) {
			Ret = Temp;
			break;
		}
	}

	if(!Ret) {
		// 끝까지 가도 없으면 역순으로 찾아봅니다.
		for (int i = Index - 1; i >= 0; --i) {
			UGenericSkill* Temp = SkillList[SkillTable[i]];
			if (Temp->Cooldown == 0) {
				Ret = Temp;
				break;
			}
		}
	}

	// Not found
	return Ret;
}
