// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericCharacter.h"
#include "Components/MeshComponent.h"

#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Generic/GenericEffect.h>
#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/UI/GageUI.h>

AGenericCharacter::AGenericCharacter()
{
	UActorComponent* Ptr;

	Ptr = GetCharacterMovement();
	check(Ptr);
	static_cast<UCharacterMovementComponent*>(Ptr)->BrakingDecelerationWalking = 2048;
	static_cast<UCharacterMovementComponent*>(Ptr)->BrakingFrictionFactor = 2;
	static_cast<UCharacterMovementComponent*>(Ptr)->MaxAcceleration = 8192;

	Ptr = GetCapsuleComponent();
	check(Ptr);
	static_cast<UCapsuleComponent*>(Ptr)->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	static_cast<UCapsuleComponent*>(Ptr)->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// 생성은 HP만 (MP, SP 생성하지 않음)
	HP.UI = CreateDefaultSubobject<UWidgetComponent>(_T("HPBar"));
	HP.UI->SetupAttachment(RootComponent);
	HP.UI->SetRelativeLocation({ 0, 0, 120 });
	UGageUI::Setup(HP.UI, _T("/Game/UI/UI_GageHP.UI_GageHP_C"));

	// 캐스트 바
	Charge.UI = CreateDefaultSubobject<UWidgetComponent>(_T("CastingBar"));
	Charge.UI->SetupAttachment(RootComponent);
	Charge.UI->SetRelativeLocation({ 0, 0, 100 });
	UGageUI::Setup(Charge.UI, _T("/Game/UI/UI_GageCast.UI_GageCast_C"));
}

AGenericCharacter::~AGenericCharacter()
{
}

void AGenericCharacter::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	Initialize();
}

void AGenericCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HPBar->OnTick(DeltaTime);
	if (IsDead) {
		return;
	}
	MoveTick();
	Act.OnTick(DeltaTime);
	Effects.OnTick(DeltaTime);

	// 쿨타임 적용
	auto Itr = SkillList.begin(), End = SkillList.end();
	while(Itr != End){
		UGenericSkill* ItrSkill = Itr.Value();
		if (ItrSkill->Cooldown != 0) {
			ItrSkill->Cooldown -= DeltaTime;
			if (ItrSkill->Cooldown <= 0) {
				ItrSkill->Cooldown = 0;
			}
		}
		++Itr;
	}
}

void AGenericCharacter::Initialize()
{
	HPBar   = Cast<UGageUI>(HP.UI->GetWidget());
	CastBar = Cast<UGageUI>(Charge.UI->GetWidget());

	if (USkeletalMeshComponent* Ptr = GetMesh()) {
		if (UDataTable* Table = LoadObject<UDataTable>(nullptr, _T("/Game/Data/Tables/Characters.Characters"))) {
			FCharacterData* Row;
			
			Row = Table->FindRow<FCharacterData>(RowName, _T("Character"));
			if (!Row) {
				Row = Table->FindRow<FCharacterData>(_T("Test"), _T("Character"));
				check(Row);
			}

			// 스킬을 Load 한 후 해당 스킬들을 등록합니다.
			int Loop = Row->UseableSkillList.Num();
			UseableSkillList.Init(nullptr, Loop);
			for (int i = 0; i < Loop; ++i) {
				UseableSkillList[i] = Cast<USkillData>(Row->UseableSkillList[i]->GetDefaultObject());
			}

			// get mesh
			if (Row->Mesh) {
				Ptr->SetSkeletalMesh(Row->Mesh);
				Ptr->SetWorldRotation(Ptr->GetComponentRotation() + Row->MeshRotationAdjust);
				Ptr->SetWorldLocation(Ptr->GetComponentLocation() + Row->MeshLocationAdjust);
			}

			// get animation
			if (Row->AnimationBase) {
				Ptr->SetAnimInstanceClass(Row->AnimationBase);
				AnimationBase = Ptr->GetAnimInstance();
			}

			// 캐스팅 모션
			if (Row->AnimationCasting) {
				CastingMotion = Row->AnimationCasting;
			}
			else CastingMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Cast.AM_Default_Cast"));

			// 공격 모션
			if (Row->AnimationAttack) {
				AttackMotion = Row->AnimationAttack;
			}
			else AttackMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Attack.AM_Default_Attack"));

			// 피격 모션
			if (Row->AnimationHit) {
				HitMotion = Row->AnimationHit;
			}
			else HitMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Hit.AM_Default_Hit"));

			// 사망 모션
			if (Row->AnimationDead) {
				DeadMotion = Row->AnimationDead;
			}
			else DeadMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Dead.AM_Default_Dead"));

			// 직업 정보
			ClassType = Row->ClassType;

			int CharacterLevel = 1; // 임시
			Source  = Row->Default;
			Current = Row->Default;

			HP.Value = Current.HP;
			MP.Value = Current.MP;
			SP.Value = Current.SP;

			RootComponent->SetWorldScale3D(Row->Scale);
		}
	}
}

void AGenericCharacter::OnHit(AGenericCharacter* InHiter)
{
	if (IsDead) {
		return;
	}

	static const FName LIST[] = {
		_T("Hit_0"),
		_T("Hit_1"),
	};
	
	// 몽타주 실행중이 아닐 때
	if (!AnimationBase->IsAnyMontagePlaying()) {
		// 몽타주 재생
		AnimationBase->Montage_Play(HitMotion);
		// 랜덤
		AnimationBase->Montage_JumpToSection(LIST[FMath::Rand() & 0b1], HitMotion);
	}
}

auto AGenericCharacter::GetType() const -> ETargetType
{
	return ETargetType::NPC;
}

auto AGenericCharacter::GetTeam() const -> ETeamType
{
	return Team;
}

auto AGenericCharacter::GetRelation(IInteractable* InOther) const -> ERelationType
{
	return IInteractable::GetRelation(InOther);
}

void AGenericCharacter::OnTargetting()
{
	// not work
}

float AGenericCharacter::GetPower() const
{
	return Current.Attack;
}

float AGenericCharacter::GetResistance() const
{
	return Current.Defence;
}

void AGenericCharacter::Dead()
{
	if (!IsDead) {
		// 모든 플래그를 끄고
		// 죽은 상태와 무시당하는 상태만 플래그를 켭니다
		IsInputMove  = false;
		IsMouseMove  = false;
		IsTargetMove = false;
		IsAutoMove   = false;
		IsJumping    = false;
		IsDead       = true;
		IsIgnore     = true;

		static const FName LIST[] = {
			_T("Dead_0"),
			_T("Dead_1"),
			_T("Dead_2"),
			_T("Dead_3"),
		};

		// Hit 몽타주 끄기
		if (AnimationBase->Montage_IsPlaying(HitMotion)) {
			AnimationBase->Montage_Stop(0.0f, HitMotion);
		}

		// 몽타주 재생
		AnimationBase->Montage_Play(DeadMotion);
		// 랜덤
		int Count = DeadMotion->CompositeSections.Num();
		AnimationBase->Montage_JumpToSection(LIST[FMath::Rand() % Count], DeadMotion);

		// 스킬 캔슬
		Act.Cancel();
		// 스킬 충전이 완료된 채 중지되어 실행도 캔슬도 되지 않는 상황이 있을 수 있어
		// 강제로 UI를 종료합니다.
		CastBar->Hide();

		// 무브먼트 종료
		GetCharacterMovement()->DisableMovement();
	}
}

void AGenericCharacter::View(const FVector& InTarget)
{
	SetActorRotation(CUtil::RemoveZ((InTarget - GetActorLocation())).GetSafeNormal().Rotation());
}

bool AGenericCharacter::Damage(UGenericSkill* InSkill, AGenericCharacter* InOther, bool UseCritical)
{
	return Damage(InSkill->Info.Power, InOther, UseCritical);
}

bool AGenericCharacter::Damage(float InDamage, AGenericCharacter* InOther, bool UseCritical)
{
	if (!InOther) return false;

	float Ciritical = 1;
	if (UseCritical) {
		if (FMath::FRand() < (Current.CriticalHit * 0.01)) {
			Ciritical = Current.CriticalMulit * 0.01;
		}
	}

	// 데미지 공식: 임시
	float Power = GetPower() * Ciritical * InDamage;
	float Resistance = InOther->GetResistance();
	if (Resistance) {
		Power /= Resistance;
	}

	InOther->HP.Value -= Power;
	if (InOther->HP.Value <= 0) {
		InOther->HP.Value = 0;
		InOther->Dead();
	}

	InOther->HPBar->Update(InOther->HP.Value, InOther->Current.HP, 3);
	return true;
}

void AGenericCharacter::UpdateStatus()
{
	Current = Source;
	// 기타 버프 구현해야됨
}

void AGenericCharacter::MoveTick()
{
	// 마우스 최우선 처리
	if (IsMouseMove) {
		MoveAuto();
	}

	// 입력 처리
	else if (IsInputMove) {
		MoveDerection();
		return;
	}
	
	// 자동 이동의 경우
	else if (IsAutoMove) {
		MoveAuto();
		return;
	}

	// 타겟 이동의 경우
	else if (IsTargetMove) {
		MoveAuto();
		// 목표 위치가 있다면 이동 후 도착했는지 검사
		if ((m_MoveTo - GetActorLocation()).SizeSquared2D() < 100) {
			IsTargetMove = false;
		}
	}
}

void AGenericCharacter::Jump()
{
	if (CanJump()) {
		Super::Jump();
		IsJumping = true;
	}
}

void AGenericCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	IsJumping = false;
}

void AGenericCharacter::MoveDerection()
{
	AddMovementInput(GetActorForwardVector() * m_Direction.X + GetActorRightVector() * m_Direction.Y);
}

void AGenericCharacter::MoveAuto()
{
	AddMovementInput(GetActorForwardVector());
}

void AGenericCharacter::ToggleAutoMove()
{
	// 자동이동 끄고 켜기
	IsAutoMove = !IsAutoMove;
	if (IsAutoMove) {
		StartAutoMove();
	}
}

void AGenericCharacter::StartTargetMove(const FVector& InTarget)
{
	// 죽어있거나 이동중이면 무시
	if (IsDead || IsInputMove) {
		return;
	}
	IsAutoMove   = false;
	IsTargetMove = true;        // set flag
	m_MoveTo     = InTarget;    // store value

	// 클릭 위치로 향하는 방향을 계산합니다.
	FVector DirTemp = InTarget - GetActorLocation();
	DirTemp.Z = 0;
	DirTemp.Normalize();
	// Z축은 무시합니다.

	// 해당 방향으로 회전하는 Rotator를 구합니다
	FRotator RotTemp = DirTemp.Rotation();

	// 해당 방향으로 회전
	SetActorRotation(FRotator{ 0, RotTemp.Yaw, 0 });
}

void AGenericCharacter::StartInputMove(const FVector2d& InInputVector, bool InFlag)
{
	// 마우스로 이동하는 경우 실제 이동은 하지 않고 방향만 관리 합니다.
	IsTargetMove = false; // 타겟 이동 정지
	IsAutoMove   = false; // 자동 이동 정지: 앞 뒤 이동 시

	// 좌우 입력은 진행하도록 수정 필요

	// 받아서 정규화
	// 나중에 Z축 처리
	m_Direction.X = InInputVector.X;
	m_Direction.Y = InInputVector.Y;

	if (!m_Direction.IsZero()) {
		m_Direction.Normalize();
	}

	// GenericInput에서 입력의 결과값을 전달받습니다.
	IsInputMove = InFlag;
}

void AGenericCharacter::StartAutoMove()
{ 
	// 플레이어만 사용되는 메소드입니다.
	// 이동시 스킬 캔슬
	Act.Cancel();

	// 입력으로 중이면 무시합니다.
	if (IsInputMove || IsMouseMove) {
		return;
	}
	IsAutoMove   = true;
	IsTargetMove = false; // 타겟 이동중이면 상태 전환됩니다.
}

void AGenericCharacter::StartMouseMove()
{
	// 플레이어만 사용되는 메소드입니다.
	// 이동시 스킬 캔슬
	Act.Cancel();

	if (IsDead) {
		return;
	}

	IsMouseMove  = true;
	IsAutoMove   = false; // 자동 이동 종료
	IsTargetMove = false; // 타겟 이동 종료
}

int AGenericCharacter::PlayAnimationAttack(int InMontageSectionIndex)
{
	return 0;
}

int AGenericCharacter::PlayAnimationDead(int InMontageSectionIndex)
{
	return 0;
}

UGenericSkill* AGenericCharacter::GetSkillInfo(USkillData* Find)
{
	return SkillList[Find];
}

void AGenericCharacter::AddPattern(USkillData* InData, int InLevel)
{
	SkillList.Add(InData, NewObject<UGenericSkill>(this));
	// 생성한 객체에 데이터 기반으로 계수 계산
	SkillList[InData]->SetData(InData, InLevel);
}

void AGenericCharacter::LearnSkill(UGenericSkill* InSkill)
{
	if (SkillList.Find(InSkill->Data)) {
		SkillList[InSkill->Data]->SetLevel(InSkill->Level); // 레벨만 다시 계산합니다.
	}
	SkillList.Add(InSkill->Data, InSkill); // 해당 스킬을 배웁니다.
}

void AGenericCharacter::RemoveSkill(USkillData* InData)
{
	if (SkillList.Find(InData)) {
		SkillList.Remove(InData); // 지웁니다.
	}
}

bool AGenericCharacter::IsLive() const
{
	return !IsDead;
}

bool AGenericCharacter::IsMoving() const
{
	// return IsAutoMove || IsInputMove || IsTargetMove || IsMouseMove;
	return GetMovementComponent()->Velocity.SquaredLength() != 0;
}

bool AGenericCharacter::IsMovingStraight() const
{
	return (IsAutoMove || IsTargetMove || IsMouseMove);
}

FVector AGenericCharacter::GetDirection() const
{
	return m_Direction;
}
