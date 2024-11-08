#pragma once

#include <LWE_WOW/Interface/Interactable.h>
#include <LWE_WOW/Common/Constants.h>

class AGenericCharacter;

class LWE_WOW_API CTarget {
public:
	CTarget(IInteractable* Parent);

	void Setting(IInteractable*); // 범용적인 Set 함수
	void SettingActor(AActor*);   // Casting 후 nullptr 또는 객체 정보
	void Unsetting();             // nullptr 초기화

	IInteractable* Get();
	ERelationType  GetRelation() const;

	bool IsSelf() const;

	operator bool();

	template<typename T> T* Get() {
		return Cast<T>(m_Other);
	}

private:
	IInteractable* m_This;
	IInteractable* m_Other;
	ERelationType  m_Relation;
};