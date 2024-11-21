#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SlotInfo.generated.h"

UCLASS()
class USlot : public UObject
{ GENERATED_BODY()
public:
	UTexture2D* Icon; // 캐스팅 안 하고 로드할수 있도록 데이터 외 따로 보관
};