#pragma once

#include <LWE_WOW/Manager/UIManager.h>
#include <LWE_WOW/Generic/GenericIcon.h>

#include "CoreMinimal.h"

enum ESlotType {
	Skill,
	Item,
};

template<ESlotType Type>
class CInventory {
public:
	static const MAX_SLOT = 40;

public:
	CInventory(UIManager* InParent);

public:
	void Setting(int Count, TSubclassOf<UUserWidget> InResource);

public:
	void Align(int InRow, int InCol, FVector2d InGap = { 0, 0 });
	void MoveTo(FVector2d InLocation);
	
private:
	FVector2d Position;

private:
	UGenericSlot* Slots[MAX_SLOT];

private:
	UUIManager* m_Parent;
};

template<ESlotType Type>
inline CInventory<Type>::CInventory(UUIManager* InParent):
	m_Parent(InParent) {}

template<ESlotType Type>
inline void CInventory<Type>::Setting(int Count, TSubclassOf<UUserWidget> InResource)
{
	ensure(Count <= MAX_SLOT);

	for (int i = 0; i < Count; ++i) {
		Slots[i] = CreateWidget<UGeneircIcon>(m_Parent->GetWorld(), InResource);
	}
}

template<ESlotType Type>
inline void CInventory<Type>::MoveTo(FVector2d InLocation)
{

}

template<ESlotType Type>
inline void CInventory<Type>::Align(int InRow, int InCol,
	FVector2d InGap)
{
	for (int y = 0; y < InCol; ++y) {
		for (int x = 0; x < InRow; ++x) {
			
			double X = Position.X +
				64 * x +  // 사이즈 임시 값
				InGap.X;  // 간격

			double X = Position.Y +
				64 * x +  // 사이즈 임시 값
				InGap.Y;  // 간격
		}
	}
}
