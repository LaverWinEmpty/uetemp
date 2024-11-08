// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <LWE_WOW/Interface/Interactable.h>
#include <LWE_WOW/Class/Target.h>
#include <LWE_WOW/Class/Buff.h>
#include <LWE_WOW/Class/Act.h>
#include <LWE_WOW/Status/CharacterInfo.h>

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Templates/SharedPointer.h"

#include "GenericCharacter.generated.h"

class UWidgetComponent;
class USkillData;
class UGageUI;

UCLASS()
class LWE_WOW_API AGenericCharacter : public ACharacter, public IInteractable {
	GENERATED_BODY()

public:
	AGenericCharacter();
	~AGenericCharacter();

public:	
	virtual void BeginPlay()               override;
	virtual void Tick(float DeltaTime)     override;	
	virtual void Jump()                    override;
	virtual void Landed(const FHitResult&) override;
	
public:
	virtual ETargetType   GetType()                   const override;
	virtual ETeamType     GetTeam()                   const override;
	virtual ERelationType GetRelation(IInteractable*) const override;
	virtual void          OnTargetting()                    override;

public:
	virtual void Initialize();

public:
	virtual void OnHit(AGenericCharacter* InHiter);

protected:
	virtual void MoveTick();

public:
	void MoveDerection();
	void MoveAuto();

public:
	void ToggleAutoMove();

public:
	virtual float GetPower()      const;
	virtual float GetResistance() const;

public:
	virtual void Dead();

public:
	virtual void View(const FVector&);

public:
	bool Damage(UGenericSkill* InSkillInfo, AGenericCharacter* InOther, bool UseCritical = true);

public:
	void StartTargetMove(const FVector&);
	void StartInputMove(const FVector2d&, bool);
	void StartAutoMove();
	void StartMouseMove();

public:
	int PlayAnimationAttack(int InMontageSectionIndex);
	int PlayAnimationDead(int InMontageSectionIndex);

public:
	UGenericSkill* GetSkillInfo(USkillData*);

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsLive() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsMoving() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsMovingStraight() const;

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetDirection() const;

public:
	// Datatable에서 데이터 찾을 때 사용되는 이름입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	FName RowName = "Test";

public:
	UPROPERTY(BlueprintReadWrite, Category = "Information")
	int Level;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	ETeamType Team = ETeamType::NONE;

public:
	FCharacterInfo Source;  // 버프 영향 받기 전 스탯
	FCharacterInfo Current; // 버프 영향 받은 후 스탯

public:
	struct Gage {
		UWidgetComponent* UI;
		float             Value;
	};
	Gage Charge; // 캐스팅 바

	// 버프로 최대체력도 증가 가능하기 때문에 이렇게 따로 처리
	Gage HP; // ┐
	Gage MP; // ├ 현재 체력 등
	Gage SP; // ┘
	// MP SP는 화면에 띄우지 않으나 직관성을 위해 유지

	UGageUI* HPBar; // 포인터 저장용
	UGageUI* CastBar; // 포인터 저장용

public:
	/*
		잠재적 이슈
		Skill이 단순 구조체이기 때문에 Info를 읽는게 스택 데이터를 읽음 
		해당 객체가 사라졌으나 버프 등 Info를 읽어야 하는 상황 발생 시 문제

		수정 완료
	*/
	
	TMap<USkillData*, UGenericSkill*> SkillList;  // 배운 스킬 목록입니다.
	TMap<EActionID,   UGenericSkill*> SkillSlots; // 바인딩 된 스킬 목록입니다.
	TArray<USkillData*>               SkillTable; // 배운 스킬 인덱싱용 배열입니다.

public:
	CTarget Target  = CTarget(this); // 타겟팅 유틸입니다.
	CBuff   Effects = CBuff(this);   // 상태이상 등의 효과 유틸입니다.
	CAct    Act     = CAct(this);    // 스킬 유틸입니다.

protected:
	FVector m_Direction; // 실제 움직이는 벡터입니다.
	FVector m_MoveTo;    // 목표 위치 벡터입니다.

public:
	UAnimInstance* AnimationBase;
	UAnimMontage*  DeadMotion;
	UAnimMontage*  AttackMotion;
	UAnimMontage*  CastingMotion;
	UAnimMontage*  HitMotion;

public:
	// 입력으로 인한 이동상태인지 확인합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsInputMove : 1 = false;

	// 입력으로 인한 이동상태인지 확인합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsMouseMove : 1 = false;

	// 타겟 이동상태인지 확인합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsTargetMove : 1 = false;

	// 자동 이동상태인지 확인합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsAutoMove : 1 = false;

	// 점프 상태인지 확인 합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsJumping : 1 = false;

	// 살아있는지 확인합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsDead : 1 = false;

	// 무시당하는 상태(은신 등)인지 확인합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsIgnore : 1 = false;

	// 회전하는 상태인지 확인합니다.
	UPROPERTY(BlueprintReadOnly, Category = "Flag") uint32 IsRotating : 1 = false;
};
