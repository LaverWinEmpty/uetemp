#pragma once

#include <LWE_WOW/Common/Constants.h>
#include "BuffInfo.generated.h"

// 상태이상 정보
USTRUCT(BlueprintType)
struct LWE_WOW_API FBuffInfo
{
	GENERATED_BODY()

	FBuffInfo();
	~FBuffInfo();

	// 계수 Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Power;      
	
	// 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Duration;   
	
	// 간격 시간: Duration과 일치하거나 그 이상이면 일회성이 됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Interval;   
	
	// 중첩 허용 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
	float Overlapped; 

public:
	// 인스턴스가 실제 지난 시간 등을 가진 정보 구조체입니다.
	struct State {
		float Duration;
		float Interval;
		float Overlapped;
	};
};