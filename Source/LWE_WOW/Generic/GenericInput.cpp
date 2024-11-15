// Fill out your copyright notice in the Description page of Project Settings.

#include "GenericInput.h"

#include "Kismet/GameplayStatics.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>
#include <LWE_WOW/Generic/GenericSkill.h>
#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/Manager/PlayerManager.h>
#include <LWE_WOW/UI/QuickSlotUI.h>

AGenericInput::AGenericInput()
{
}

AGenericInput::~AGenericInput() {}

void AGenericInput::Initialize(UEnhancedInputComponent* InEIC)
{
	// 기본 기능들
	InEIC->BindAction(m_IA[IA_LOOK], ETriggerEvent::Triggered, this, &AGenericInput::OnInput<FVector2d>);
	InEIC->BindAction(m_IA[IA_ZOOM], ETriggerEvent::Triggered, this, &AGenericInput::OnInput<float>);
	InEIC->BindAction(m_IA[IA_JUMP], ETriggerEvent::Started,   this, &AGenericInput::OnInput<bool>);
	InEIC->BindAction(m_IA[IA_AUTO], ETriggerEvent::Started,   this, &AGenericInput::OnInput<bool>);

	// UI 관련
	InEIC->BindAction(m_IA[IA_UI_TOGGLE_INVENTORY], ETriggerEvent::Started, this, &AGenericInput::OnInput<bool>);
	InEIC->BindAction(m_IA[IA_UI_TOGGLE_EQUIPMENT], ETriggerEvent::Started, this, &AGenericInput::OnInput<bool>);
	InEIC->BindAction(m_IA[IA_UI_TOGGLE_SKILL],     ETriggerEvent::Started, this, &AGenericInput::OnInput<bool>);

	// BindAction, bool Started / Completed 묶어서 
	BindPushKey(InEIC, IA_MOVE_L);
	BindPushKey(InEIC, IA_MOVE_R);
	BindPushKey(InEIC, IA_MOVE_F);
	BindPushKey(InEIC, IA_MOVE_B);
	BindPushKey(InEIC, IA_SHIFT_L);
	BindPushKey(InEIC, IA_CTRL_L);
	BindPushKey(InEIC, IA_ALT_L);
	BindPushKey(InEIC, IA_MOUSE_L);
	BindPushKey(InEIC, IA_MOUSE_R);

	for (int i = IA_COMMAND_0; i < IA_COMMAND_9; ++i) {
		BindPushKey(InEIC, static_cast<EActionID>(i));
	}

	Mapping                      (IA_ZOOM, CDefault::DefKey::ZOOM_OUT);
	Mapping<UInputModifierNegate>(IA_ZOOM, CDefault::DefKey::ZOON_IN);

	Mapping(IA_JUMP, CDefault::DefKey::JUMP);
	Mapping(IA_LOOK, CDefault::DefKey::MOUSE);

	Mapping(IA_MOVE_F, CDefault::DefKey::MOVE_F);
	Mapping(IA_MOVE_R, CDefault::DefKey::MOVE_R);
	Mapping(IA_MOVE_B, CDefault::DefKey::MOVE_B);
	Mapping(IA_MOVE_L, CDefault::DefKey::MOVE_L);

	Mapping(IA_CTRL_L,  EKeys::LeftControl);
	Mapping(IA_SHIFT_L, EKeys::LeftShift);
	Mapping(IA_ALT_L,   EKeys::LeftAlt);

	Mapping(IA_MOUSE_L, EKeys::LeftMouseButton);
	Mapping(IA_MOUSE_R, EKeys::RightMouseButton);

	// 임시
	Mapping(IA_AUTO, EKeys::CapsLock);

	// 임시
	Mapping(IA_COMMAND_0,  EKeys::One   );
	Mapping(IA_COMMAND_1,  EKeys::Two   );
	Mapping(IA_COMMAND_2,  EKeys::Three );
	Mapping(IA_COMMAND_3,  EKeys::Four  );
	Mapping(IA_COMMAND_4,  EKeys::Five  );
	Mapping(IA_COMMAND_5,  EKeys::Six   );
	Mapping(IA_COMMAND_6,  EKeys::Seven );
	Mapping(IA_COMMAND_7,  EKeys::Eight );
	Mapping(IA_COMMAND_8,  EKeys::Nine  );
	Mapping(IA_COMMAND_9,  EKeys::Zero  );
	Mapping(IA_COMMAND_10, EKeys::Hyphen);
	Mapping(IA_COMMAND_11, EKeys::Equals);
	Mapping(IA_COMMAND_12, EKeys::F1    );
	Mapping(IA_COMMAND_13, EKeys::F2    );
	Mapping(IA_COMMAND_14, EKeys::F3    );
	Mapping(IA_COMMAND_15, EKeys::F4    );
	Mapping(IA_COMMAND_16, EKeys::F5    );
	Mapping(IA_COMMAND_17, EKeys::F6    );
	Mapping(IA_COMMAND_18, EKeys::F7    );
 	Mapping(IA_COMMAND_19, EKeys::F8    );
	Mapping(IA_COMMAND_20, EKeys::F9    );
	Mapping(IA_COMMAND_21, EKeys::F10   );
	Mapping(IA_COMMAND_22, EKeys::F11   );
	Mapping(IA_COMMAND_23, EKeys::F12   );

	// UI 매핑
	Mapping(IA_UI_TOGGLE_INVENTORY, EKeys::I);
	Mapping(IA_UI_TOGGLE_EQUIPMENT, EKeys::U);
	Mapping(IA_UI_TOGGLE_SKILL,     EKeys::K);

	// 안전한 생성을 위해 Cleanup -> Setup
	if (UUIManager* Instance = UUIManager::Instance(this)) {
		Instance->Setup();
	}
}

void AGenericInput::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (CUtil::Check((m_Player = Cast<AGenericCharacter>(InPawn)))) {
		ensure(false);
		return;
	}

	// IMC 연결
	if (APlayerController* PC = Cast<APlayerController>(m_Player->GetController())) {
		if (CUtil::Check(PC)) {
			ensure(false);
			return;
		}
		UEnhancedInputLocalPlayerSubsystem* Sub = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (CUtil::Check(Sub)) {
			ensure(false);
			return;
		}
		Sub->AddMappingContext(m_IMC, 0);
	}

	m_Camera = MakeShareable(new CCamera);
	m_Camera->Setup(m_Player);

	m_Player->RowName = UPlayerManager::Instance(this)->PlayerNameOfDatatableRow;

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (CUtil::Check(EIC)) {
		ensure(false);
		return;
	}

	Initialize(EIC);
}

void AGenericInput::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (CUtil::Check(EIC)) {
		ensure(false);
		return;
	}

	m_IA.Init(nullptr, IA_END);
	for (int i = IA_BEGIN; i < IA_END; ++i)
		CreateAction(static_cast<EActionID>(i));

	// Set
	m_IA[IA_ZOOM]->ValueType = EInputActionValueType::Axis1D;
	m_IA[IA_LOOK]->ValueType = EInputActionValueType::Axis2D;

	// IMC 생성 후 검사
	if (!m_IMC) {
		if (CUtil::Check((m_IMC = NewObject<UInputMappingContext>()), CUtil::ON_DISPLAY)) {
			ensure(false);
			return;
		}
	}

	// 마우스 출력
	bShowMouseCursor = true;
}

void AGenericInput::OnMouseLeftClick(bool InFlag)
{
	CUtil::SetFlag(m_Mod, MOD_MOUSE_L, InFlag);
	TOptional<FHitResult> Hit = OnMouseClick();
	if (InFlag) {
		PostMouseLeftClick(Hit);
	}

	else {
		FVector2d MouseLeftClickCurrent;
		GetMousePosition(MouseLeftClickCurrent.X, MouseLeftClickCurrent.Y);

		// 단순 클릭이라 판단 되면
		if (m_Left.Screen == MouseLeftClickCurrent) {

			// 눌린 게 있는지 확인
			if (m_Left.Flag && m_Left.Target) {
				// 타겟 지정 후
				SetTargetOfPlayer(m_Left.Target);

				// 상호작용 없는 객체 == NPC 등이라면
				if (m_Player->Target.GetRelation() == ERelationType::NONE) {
					FVector Vector = m_Player->Target.Get<AGenericCharacter>()->GetActorForwardVector();
					// 해당 객체 바로 앞까지 이동합니다.
					MoveTarget(FVector{ Vector.X, Vector.Y, 0 });
				}
			}
			else UnsetTargetOfPlayer(); // 없으면 타겟 초기화

			m_Left.Flag = false; // 기록 초기화
		}
		m_Player->IsMouseMove = false; // 마우스 해제: 마우스 이동 정지
	}
}

void AGenericInput::OnMouseRightClick(bool InFlag)
{
	CUtil::SetFlag(m_Mod, MOD_MOUSE_R, InFlag);
	TOptional<FHitResult> Hit = OnMouseClick();
	if (InFlag) {
		PostMouseRightClick(Hit);
	}

	// 마우스 해제 시 해당 위치로 이동할지 판단합니다.
	// (타겟 이동 활성화)
	else {
		// 입력 해제인 경우
		FVector2d MouseRightClickCurrent;
		GetMousePosition(MouseRightClickCurrent.X, MouseRightClickCurrent.Y);

		// 단순 클릭이라 판단 되면
		if (m_Right.Screen == MouseRightClickCurrent) {

			// 눌린게 있는지 확인
			if (m_Right.Flag) {
				// 적이면 공격하기 위해 타겟팅
				if (m_Right.Target && m_Right.Target->GetRelation(m_Player) == ERelationType::HARM) {
					SetTargetOfPlayer(m_Right.Target);
				}
				// 타겟이 없으면(IInteractable 객체가 아니면) 이동
				else if (!m_Right.Target) {
					MoveTarget(FVector{ m_Right.World.X, m_Right.World.Y, 0 });
				}
			}

			else m_Player->IsTargetMove = false; // 화면 회전 시 이동 정지합니다
			m_Right.Flag = false; // 기록 초기화
		}
		m_Player->IsMouseMove = false; // 마우스 해제: 마우스 이동 정지
	}
}

void AGenericInput::PlayerAction(EActionID InID)
{
	// 단축키에 등록된 스킬을 가져와서 사용합니다.
	PlayerAction(UUIManager::Instance(this)->GetSkillSlot(InID)->GetSlotInfo(InID));
}

void AGenericInput::PlayerAction(UGenericSkill* InSkill)
{
	if (!InSkill) {
		ERROR(0, "Skill is Nullpointer");
		return;
	}

	CAct::EResult Result = m_Player->Act.Start(InSkill);

	// 스킬 시전 성공 시
	if (CAct::IsOK(Result)) {
		// 시야 제한이 있는 스킬의 경우 스킬을 바라보기 위해 적 방향으로 회전 (타겟이 있는 경우)
		if ((InSkill->Data->Option & static_cast<uint8>(ESkillFlag::LIMIT_VIEW)) &&
			m_Player->Target && !m_Player->Target.IsSelf()) {
			// 카메라 고정 적용됨
			m_Player->View(m_Player->Target.Get<AActor>()->GetActorLocation());
		}
	}

	else {
		UUIManager::Instance(this)->SetMessageText(CAct::GetMessage(Result));
	}
}

void AGenericInput::SetTargetOfPlayer(IInteractable* InOther)
{
	AActor* Actor = Cast<AActor>(InOther); // 검증
#if WITH_EDITOR
	ensure(Actor);
#endif

	m_Player->Target.Setting(InOther);
	// 입력으로 인한 타겟 지정 == 플레이어 -> UI 출력
	UUIManager::Instance(this)->SetTargetInfo(Actor);
}

void AGenericInput::UnsetTargetOfPlayer()
{
	m_Player->Target.Unsetting();
	// 입력으로 인한 타겟 지정 == 플레이어 -> UI 출력
	UUIManager::Instance(this)->SetTargetInfo(nullptr);
}

TOptional<FHitResult> AGenericInput::OnMouseClick()
{
	// 클릭 검사합니다
	bool bCheck = CUtil::CheckFlag(m_Mod, MOD_MOUSE_L | MOD_MOUSE_R);
	if (bCheck) {
		m_Player->StartMouseMove(); // 이동
		LookFollow();               // 이동 시 카메라 동기화
		return NullOpt;             // 이동 시 작동 종료
	}

	FHitResult Result;
	if (GetHitResultUnderCursorForObjects(m_Trace, true, Result)) {
		return Result;
	}

	//FVector2d Screen;
	//FVector WorldOrigin, WorldDirection;

	//GetMousePosition(Screen.X, Screen.Y);

	//UGameplayStatics::DeprojectScreenToWorld(this, Screen, WorldOrigin, WorldDirection);

	//// 레이캐스트를 위한 끝 점 계산
	//FVector TraceEnd = WorldOrigin + (WorldDirection * 10000);

	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(this); // 자기 자신은 무시

	//FHitResult HitResult;
	//GetWorld()->LineTraceSingleByChannel(HitResult, WorldOrigin, TraceEnd, ECC_Visibility, QueryParams);

	//// 검증
	//if (AActor* Hit = HitResult.GetActor()) {
	//	// 클릭 가능한 객체일 때
	//	if (Cast<IInteractable>(HitResult.GetActor())) {
	//		return HitResult;
	//	}
	//}
	return NullOpt;
}

void AGenericInput::PostMouseLeftClick(const TOptional<FHitResult>& InHit)
{
	// InHit이 비어있지 않은 경우 == 마우스 이동하기 위한 입력이 아닌 경우
	// 클릭을 검사합니다.
	if (InHit) {
		m_Left.World  = InHit->Location;
		m_Left.Target = Cast<IInteractable>(InHit->GetActor());
		m_Left.Flag   = true;
	}
	// 마우스 위치를 기록합니다
	GetMousePosition(m_Left.Screen.X, m_Left.Screen.Y);
}

void AGenericInput::PostMouseRightClick(const TOptional<FHitResult>& InHit)
{
	// InHit이 비어있지 않은 경우 == 마우스 이동하기 위한 입력이 아닌 경우
	// 클릭을 검사합니다.
	if (InHit) {
		m_Right.World  = InHit->Location;
		m_Right.Target = Cast<IInteractable>(InHit->GetActor());
		m_Right.Flag   = true;
	}
	// 마우스 위치를 기록합니다
	GetMousePosition(m_Right.Screen.X, m_Right.Screen.Y);
}

const UInputAction* const AGenericInput::GetInputAction(EActionID InIndex) const
{
	return m_IA[InIndex];
}

auto AGenericInput::GetInputActionID(const UInputAction* const InPtr) const->EActionID
{
	return m_Table[InPtr];
}

void AGenericInput::Mapping(EActionID InID, FKey InKey)
{
	m_IMC->MapKey(m_IA[InID], InKey);
}

void AGenericInput::Unmapping(EActionID InID, FKey InKey)
{
	m_IMC->UnmapKey(m_IA[InID], InKey);
}

void AGenericInput::CreateAction(EActionID InID)
{
	// 생성 후 저장
	m_IA[InID] = NewObject<UInputAction>();

	// 매칭
	m_Table.Add(m_IA[InID].Get(), InID);
}

void AGenericInput::BindPushKey(UEnhancedInputComponent* InEIC, EActionID InID)
{
	InEIC->BindAction(m_IA[InID], ETriggerEvent::Started,   this, &AGenericInput::OnInput<bool>);
	InEIC->BindAction(m_IA[InID], ETriggerEvent::Completed, this, &AGenericInput::OnInput<bool>);
}

bool AGenericInput::PostInput(bool InValue, EActionID InID, ETriggerEvent InType)
{
	// 마우스 클릭 우선 처리
	switch (InID) {
		case IA_MOUSE_L: OnMouseLeftClick(InValue);  break;
		case IA_MOUSE_R: OnMouseRightClick(InValue); break;

		// 스킬 사용 가능
		// 죽은 상태면 내부에서 메세지 출력
		case IA_COMMAND_0:
		case IA_COMMAND_1:
		case IA_COMMAND_2:
		case IA_COMMAND_3:
		case IA_COMMAND_4:
		case IA_COMMAND_5:
		case IA_COMMAND_6:
		case IA_COMMAND_7:
		case IA_COMMAND_8:
		case IA_COMMAND_9:
		case IA_COMMAND_10:
		case IA_COMMAND_11:
		case IA_COMMAND_12:
		case IA_COMMAND_13:
		case IA_COMMAND_14:
		case IA_COMMAND_15:
		case IA_COMMAND_16:
		case IA_COMMAND_17:
		case IA_COMMAND_18:
		case IA_COMMAND_19:
			if (InValue)
				PlayerAction(InID);
			break;

		case IA_UI_TOGGLE_INVENTORY: UUIManager::Instance(this)->Toggle(UUIManager::UI_INVENTORY); break;
		case IA_UI_TOGGLE_EQUIPMENT: UUIManager::Instance(this)->Toggle(UUIManager::UI_EQUIPMENT); break;
		case IA_UI_TOGGLE_SKILL:     UUIManager::Instance(this)->Toggle(UUIManager::UI_SKILL);     break;
	}

	// 죽어있으면 입력 불가
	if (m_Player->IsDead)
		return false;

	switch (InID) {
		case IA_JUMP: Jump(); break;
		case IA_AUTO: m_Player->ToggleAutoMove(); break;

		case IA_MOVE_F: Move(FVector2d(FVector::ForwardVector.X,  FVector::ForwardVector.Y),  InValue); break;
		case IA_MOVE_B: Move(FVector2d(FVector::BackwardVector.X, FVector::BackwardVector.Y), InValue); break;
		case IA_MOVE_L: Move(FVector2d(FVector::LeftVector.X,     FVector::LeftVector.Y),     InValue); break;
		case IA_MOVE_R: Move(FVector2d(FVector::RightVector.X,    FVector::RightVector.Y),    InValue); break;

		case IA_ALT_L:   CUtil::SetFlag(m_Mod, MOD_SHIFT_L, InValue); break;
		case IA_CTRL_L:  CUtil::SetFlag(m_Mod, MOD_CTRL_L,  InValue); break;
		case IA_SHIFT_L: CUtil::SetFlag(m_Mod, MOD_SHIFT_L, InValue); break;
	}

	return true;
}

bool AGenericInput::PostInput(float InValue, EActionID InID, ETriggerEvent InType)
{
	if (InID == IA_ZOOM) {
		Zoom(InValue);
	}

	if (m_Player->IsDead)
		return false;
	return true;
}

bool AGenericInput::PostInput(const FVector2d& InValue, EActionID InID, ETriggerEvent InType)
{
	if (InID == IA_LOOK) {
		Look(InValue, m_Mod);
	}

	if (m_Player->IsDead)
		return false;
	return true;
}

void AGenericInput::Look(const FVector2d& InValue, EModFlag InFlag)
{
	// 화면을 클릭하면 카메라가 회전합니다
	if (InFlag & (EModFlag::MOD_MOUSE_L | EModFlag::MOD_MOUSE_R)) {
		m_Camera->Rotate(InValue * CameraSpeed * CUtil::GetDeltaTime(this, false));
	}

	// 오른쪽 버튼을 누르면 캐릭터가 카메라와 동기화 됩니다 (고정 기능)
	if (InFlag & EModFlag::MOD_MOUSE_R) {
		LookFollow();
		// 타겟 이동 정지합니다
		m_Player->IsTargetMove = false;
	}
}

void AGenericInput::LookFollow()
{
	// 죽어있으면 회전하지 않음
	if (m_Player->IsDead) {
		return;
	}

	// 캐릭터를 카메라 회전과 일치시킵니다.
	FRotator CameraRotator = m_Camera->GetCameraArmComponent()->GetComponentRotation();
	m_Player->SetActorRotation(FRotator{ 0, CameraRotator.Yaw, 0 });
	m_Camera->GetCameraArmComponent()->SetWorldRotation(CameraRotator);
}

void AGenericInput::Jump()
{
	m_Player->Act.Cancel();
	m_Player->Jump();
}

void AGenericInput::Zoom(float InValue)
{
	m_Camera->Zoom(InValue * CameraZoomUnit);
	if (m_Camera->ZoomValue < PLAYER_MESH_HIDE_ZOOM) {
		m_Player->GetMesh()->SetOwnerNoSee(true);
	}
	else {
		m_Player->GetMesh()->SetOwnerNoSee(false);
	}
}

void AGenericInput::MoveTarget(const FVector& InTarget)
{
	// 카메라의 위치를 유지하기 위해 저장합니다.
	FRotator CameraRotator = m_Camera->GetCameraArmComponent()->GetComponentRotation();

	m_Player->StartTargetMove(InTarget);

	// 카메라 각도를 유지합니다.
	m_Camera->GetCameraArmComponent()->SetWorldRotation(CameraRotator);

	// 플레이어 이동 시 스킬 시전 취소
	m_Player->Act.Cancel();
}

USpringArmComponent* AGenericInput::GetCameraArm()
{
	if (m_Camera)
		return m_Camera->GetCameraArmComponent();
	return nullptr;
}

void AGenericInput::Move(const FVector2d& InDirection, bool InInput)
{
	if (InInput) {
		// 방향 벡터를 추가
		m_PlayerInputWay.X += InDirection.X;
		m_PlayerInputWay.Y += InDirection.Y;
	}
	else {
		// 방향 벡터를 제거
		m_PlayerInputWay.X -= InDirection.X;
		m_PlayerInputWay.Y -= InDirection.Y;

		// 입력 해제 후 방향 값이 없으면 입력 이동 종료 알림
		if (!m_PlayerInputWay) {
			m_Player->StartInputMove(FVector2d{ 0, 0 }, false);
			return;
		}
	}

	// 이동 상태면
	// 각각 X, Y를 1, 0, -1 로 변환하고 전달합니다.
	float X = m_PlayerInputWay.X;
	float Y = m_PlayerInputWay.Y;
	if (m_PlayerInputWay) {
		// X
		if      (X >  1) X =  1; //  1로 보정
		else if (X < -1) X = -1; // -1로 보정
		// Y
		if      (Y >  1) Y =  1; //  1로 보정
		else if (Y < -1) Y = -1; // -1로 보정
	}
	m_Player->StartInputMove(FVector2d{ X, Y }, true);

	// 플레이어 이동 시 스킬 시전 취소
	m_Player->Act.Cancel();
}



//// 필요 시 메쉬나 컴포넌트 변경
//UStaticMeshComponent* MeshComponent = m_Player->FindComponentByClass<UStaticMeshComponent>();
//if (MeshComponent)
//{
//	UStaticMesh* NewMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Resource/Rig/Fantasy_Pack/Characters/Warrior/Mesh/SK_Warrior.SK_Warrior"));
//	if (NewMesh) {
//		MeshComponent->SetStaticMesh(NewMesh);
//	}
//}

//// 카메라의 위치를 유지하기 위해 저장합니다.
//FRotator CameraRotator = m_Camera->GetCameraArmComponent()->GetComponentRotation();

//// 스킬 시전 방향으로 회전합니다.
//
//AGenericCharacter* Temp = m_Player->Target.Get<AGenericCharacter>();
//check(Temp);
//
//FVector Direction = Temp->GetActorLocation() - m_Player->GetActorLocation();
//Direction.Z = 0; // Z축은 무시합니다.
//Direction.Normalize();
//m_Player->SetActorRotation(Direction.Rotation());

//// 카메라 각도를 유지합니다.
//m_Camera->GetCameraArmComponent()->SetWorldRotation( CameraRotator );