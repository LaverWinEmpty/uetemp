#include "Target.h"

#include <LWE_WOW/Generic/GenericCharacter.h>
#include <LWE_WOW/Manager/UIManager.h>

CTarget::CTarget(IInteractable* InParent) : m_This(InParent), m_Other(nullptr), m_Relation(ERelationType::NONE)
{
}

void CTarget::Setting(IInteractable* InTarget)
{
	m_Other = InTarget;
	if (m_Other) {
		m_Relation = m_This->GetRelation(InTarget);
		m_Other->OnTargetting();
	}
	else m_Relation = ERelationType::NONE;
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
	return m_Other;
}

ERelationType CTarget::GetRelation() const
{
	return m_Relation;
}

bool CTarget::IsSelf() const
{
	return m_This == m_Other;
}

CTarget::operator bool()
{
	return m_Other != nullptr;
}

