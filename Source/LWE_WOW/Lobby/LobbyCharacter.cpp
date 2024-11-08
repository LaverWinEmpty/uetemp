// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

#include <LWE_WOW/Data/CharacterData.h>

ALobbyCharacter::ALobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALobbyCharacter::Initialize()
{
	// 나머지 필요 없으므로 모션만 가져옵니다.
	if (USkeletalMeshComponent* Ptr = GetMesh()) {
		if (UDataTable* Table = LoadObject<UDataTable>(nullptr, _T("/Game/Data/Tables/Characters.Characters"))) {
			FCharacterData* Row = Table->FindRow<FCharacterData>(RowName, _T("Character"));
			check(Row);

			if (Row->AnimationBase) {
				Ptr->SetAnimInstanceClass(Row->AnimationBase);
				AnimationBase = Ptr->GetAnimInstance();
			}
			else check(false);
		}
		else check(false);
	}

	CastingMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Cast.AM_Default_Cast"));
	AttackMotion  = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Attack.AM_Default_Attack"));
}

void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);
}
