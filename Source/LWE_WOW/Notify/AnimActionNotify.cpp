// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimActionNotify.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Generic/GenericEffect.h>

void UAnimActionNotify::Notify(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(InMeshComp, InAnimation, EventReference);

	AActor* Owner = InMeshComp->GetOwner();
#if WITH_EDITOR
	// 에디터 프리뷰
	if (GIsEditor && Owner && Owner->GetWorld() != GWorld)
		return; 
#endif

	AGenericCharacter* Character = Cast<AGenericCharacter>(Owner);
	if (Character) {
		Character->Act.OnNotify();
	}
}