// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericCharacter.h"
#include "Components/MeshComponent.h"

#include <LWE_WOW/Common/Util.h>
#include <LWE_WOW/Common/Constants.h>
#include <LWE_WOW/Data/CharacterData.h>
#include <LWE_WOW/Generic/GenericEffect.h>
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
}

void AGenericCharacter::Initialize()
{
		if (USkeletalMeshComponent* Ptr = GetMesh()) {
		if (UDataTable* Table = LoadObject<UDataTable>(nullptr, _T("/Game/Data/Tables/Characters.Characters"))) {
			FCharacterData* Row;
			
			Row = Table->FindRow<FCharacterData>(RowName, _T("Character"));
			if (!Row) {
				Row = Table->FindRow<FCharacterData>(_T("Test"), _T("Character"));
				check(Row);
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
			else AttackMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Cast.AM_Default_Attck"));

			// get skill list
			int Loop = Row->UseableSkillList.Num();

			SkillTable.Init(nullptr, Loop);
			for (int i = 0; i < Loop; ++i) {
				int SkillLevel = 1; // 임시

				SkillTable[i] = Row->UseableSkillList[i]->GetDefaultObject<USkillData>();

				SkillList.Add(SkillTable[i], {});
				SkillList[SkillTable[i]].Level = SkillLevel;
				SkillList[SkillTable[i]].Data  = SkillTable[i];
				SkillList[SkillTable[i]].Info  = SkillTable[i]->Calculate(Level);
			}

			int CharacterLevel = 1; // 임시
			Source = Row->Default;
			Current = Row->Default;

			HP.Value = Current.HP;
			MP.Value = Current.MP;
			SP.Value = Current.SP;
		}
	}
	DeadMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Dead.AM_Default_Dead"));

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	HPBar   = Cast<UGageUI>(HP.UI->GetWidget());
	CastBar = Cast<UGageUI>(Charge.UI->GetWidget());
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
	return Current.Attck;
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

		// 몽타주 재생
		AnimationBase->Montage_Play(DeadMotion);
		// 랜덤
		AnimationBase->Montage_JumpToSection(LIST[FMath::Rand() & 0b11], DeadMotion);

		// 무브먼트 종료
		GetCharacterMovement()->DisableMovement();
	}
}

bool AGenericCharacter::Damage(const FSkillInfo& InSkillInfo, AGenericCharacter* InOther, bool UseCritical)
{
	if (!InOther) return false;

	float Ciritical = 1;
	if (UseCritical) {
		if (FMath::FRand() < (Current.CriticalHit * 0.01)) {
			Ciritical = Current.CriticalMulit * 0.01;
		}
	}
		
	// 데미지 공식: 임시
	float Power = GetPower() * Ciritical * InSkillInfo.Power;
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

void AGenericCharacter::StartTargetMove(const FVector& InTarget)
{
	// 이동중이면 무시
	if (IsInputMove) {
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
	// 입력으로 중이면 무시합니다.
	if (IsInputMove || IsMouseMove) {
		return;
	}
	IsAutoMove   = true;
	IsTargetMove = false; // 타겟 이동중이면 상태 전환됩니다.
}

void AGenericCharacter::StartMouseMove()
{
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

auto AGenericCharacter::GetSkillInfo(USkillData* Find)->Skill
{
	return SkillList[Find];
}

bool AGenericCharacter::IsLive() const
{
	return !IsDead;
}

bool AGenericCharacter::IsMoving() const
{
	return (IsAutoMove || IsInputMove || IsTargetMove || IsMouseMove);
}

bool AGenericCharacter::IsMovingStraight() const
{
	return (IsAutoMove || IsTargetMove || IsMouseMove);
}

FVector AGenericCharacter::GetDirection() const
{
	return m_Direction;
}
