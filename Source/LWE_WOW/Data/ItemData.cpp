#include "ItemData.h"

#include "GameFramework/Character.h"

UPrimitiveComponent* FItemData::CreateMeshComponent(ACharacter* InParent)
{
	UPrimitiveComponent* RetComp;

	switch (ItemMeshType) {
		case EItemMeshType::STATIC: {
			UStaticMeshComponent* Temp = NewObject<UStaticMeshComponent>(InParent, Name);
			Temp->SetStaticMesh(StaticMesh);
			RetComp = Temp;
		}
		break;

		case EItemMeshType::SKELETAL: {
			USkeletalMeshComponent* Temp = NewObject<USkeletalMeshComponent>(InParent, Name);
			Temp->SetSkeletalMesh(SkeletalMesh);
			RetComp = Temp;
		}
		break;

		default:
			return nullptr;
	}

	RetComp->SetWorldScale3D(Scale);
	RetComp->SetWorldLocation(MeshLocationAdjust);
	RetComp->SetWorldRotation(MeshRotationAdjust);
	RetComp->SetupAttachment(InParent->GetMesh(), SocketName);
	RetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RetComp->RegisterComponent();

	return RetComp;
}
