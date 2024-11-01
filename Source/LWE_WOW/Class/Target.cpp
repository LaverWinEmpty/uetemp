#include "Target.h"

#include <LWE_WOW/Manager/UIManager.h>

CTarget::CTarget(IInteractable* InParent) : This(InParent), Other(nullptr), Relation(ERelationType::NONE)
{
}

void CTarget::Setting(IInteractable* InTarget)
{
	Other = InTarget;
	if (Other) {
		Relation = This->GetRelation(InTarget);
		Other->OnTargetting();
	}
	else Relation = ERelationType::NONE;

	if (AActor* Actor = Cast<AActor>(This)) {
		// nullptr 허용
		UUIManager::Instance(Actor)->SetTargetInfo(Cast<AActor>(Other));
	}

}

void CTarget::SettingActor(AActor* InTarget)
{
	Setting(Cast<IInteractable>(InTarget));
}

void CTarget::Unsetting()
{
	Setting(static_cast<IInteractable*>(nullptr));
}

IInteractable* CTarget::Get()
{
	return Other;
}

AActor* CTarget::GetTargettingActor()
{
	return Cast<AActor>(Other);
}

ERelationType CTarget::GetRelation() const
{
	return Relation;
}

CTarget::operator bool()
{
	return Other != nullptr;
}

