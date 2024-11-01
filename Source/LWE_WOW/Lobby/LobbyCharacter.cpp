// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

ALobbyCharacter::ALobbyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALobbyCharacter::BeginPlay()
{
	ACharacter::BeginPlay();

	if (UDataTable* Table = LoadObject<UDataTable>(nullptr, _T("/Game/Data/Tables/Characters.Characters"))) {


		if (FCharacterData* Row = Table->FindRow<FCharacterData>(RowName, _T("LobbyCharacter"))) {
			AnimationBase = GetMesh()->GetAnimInstance();

			// 공격 모션
			if (Row->AnimationAttack) {
				AttackMotion = Row->AnimationAttack;
			}
			else AttackMotion = LoadObject<UAnimMontage>(nullptr, _T("/Game/Animations/AM_Default_Cast.AM_Default_Attck"));
		}
	}
}

void ALobbyCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);
}
