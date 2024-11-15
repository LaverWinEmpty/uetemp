// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <type_traits>
#include "CoreMinimal.h"

/*
	macros
*/
#define LOG(OnDisplay, Format, ...) do {\
	if(OnDisplay) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(_T(Format), ##__VA_ARGS__)); }\
	else          { UE_LOG(LogTemp, Log, _T(Format), ##__VA_ARGS__); }\
} while(false)

#define WARNING(OnDisplay, Format, ...) do {\
	if(OnDisplay) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(_T(Format), ##__VA_ARGS__)); }\
	else          { UE_LOG(LogTemp, Warning, _T(Format), ##__VA_ARGS__); }\
} while(false)

#define ERROR(OnDisplay, Format, ...) do {\
	if(OnDisplay) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(_T(Format), ##__VA_ARGS__)); }\
	else          { UE_LOG(LogTemp, Error, _T(Format), ##__VA_ARGS__); }\
} while(false)

#define PRINT(x) do {\
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(_T("%s"), *((x)->ToString()))); \
 } while(false)


/**
 * 
 */
struct LWE_WOW_API CUtil
{
	CUtil() = delete;

	using LogOption = int;
	enum  EFlagLogOption : int8 {
		NO_LOG = 0,

		ON_FAILED_TO_CONSOLE    = 1,
		ON_FAILED_TO_DISPLAY    = 2,
		ON_SUCCEEDED_TO_CONSOLE = 4,
		ON_SUCCEEDED_TO_DISPLAY = 8,

		ON_FAILED    = ON_FAILED_TO_CONSOLE | ON_FAILED_TO_DISPLAY,
		ON_SUCCEEDED = ON_SUCCEEDED_TO_CONSOLE | ON_SUCCEEDED_TO_DISPLAY,

		ON_DISPLAY = ON_FAILED_TO_DISPLAY | ON_SUCCEEDED_TO_DISPLAY,
		ON_CONSOLE = ON_FAILED_TO_CONSOLE | ON_SUCCEEDED_TO_CONSOLE,

		ON_LOG = ON_FAILED | ON_SUCCEEDED 
	};

	inline static void SetTimeScale(double In) {
		TimeScale = In;
	}

	inline static double GetDeltaTime(const AActor* In, bool bTimeScale = true) {
		double Sec = In->GetWorld()->GetDeltaSeconds();
		if (bTimeScale) {
			Sec *= TimeScale;
		}
		return Sec;
	}

	inline static double TimeScale = 1.0;

	/*
		true: error

		if(Check(object)) {
			check(false);
		}
		TO DO
		꼴로 사용
	*/
	template<typename T> inline static bool Check(T         InObject,
                                                  LogOption InFlag = ON_SUCCEEDED_TO_CONSOLE | ON_FAILED_TO_DISPLAY)
	{
		bool Valid;

		if constexpr (TPointerIsConvertibleFromTo<T, UObject*>::Value) {
			Valid = IsValid(InObject);
		}
		else Valid = InObject != nullptr;

		if constexpr (TPointerIsConvertibleFromTo<T, UObject*>::Value) {
			// invalid
			if (!Valid) {
				if ((InFlag & ON_FAILED)) {
					if (InObject != nullptr) {
						if ((InFlag & ON_FAILED_TO_CONSOLE)) WARNING(false, "[%s] is destroyed", *InObject->GetName());
						if ((InFlag & ON_FAILED_TO_DISPLAY)) WARNING(true,  "[%s] is destroyed", *InObject->GetName());
					}
					else {
						if ((InFlag & ON_FAILED_TO_CONSOLE)) ERROR(false, "Null Pointer Exception");
						if ((InFlag & ON_FAILED_TO_DISPLAY)) ERROR(true,  "Null Pointer Exception");
					}
				}
				return true;
			}

			// valid
			else if ((InFlag & ON_SUCCEEDED)) {
				if ((InFlag & ON_SUCCEEDED_TO_CONSOLE)) LOG(false, "[%s] is OK", *InObject->GetName());
				if ((InFlag & ON_SUCCEEDED_TO_DISPLAY)) LOG(true,  "[%s] is OK", *InObject->GetName());
			}
			return false;
		}

		else return !Test(Valid, _T(""), InFlag);
	}

	/*
		nullptr 체크
		if(Test(object = GetObject())) {
			TO DO
		}
		else check(false);
		꼴로 사용
	*/
	static bool Test(void* InObject,
		const TCHAR* const InTag = nullptr,
		LogOption          InFlag = ON_SUCCEEDED_TO_CONSOLE | ON_FAILED_TO_DISPLAY);

	static bool Test(bool  InValue,
		const TCHAR* const InTag = nullptr,
		LogOption          InFlag = ON_SUCCEEDED_TO_CONSOLE | ON_FAILED_TO_DISPLAY);

	/*
		@return true:  _T("True")
		@return false: _T("False")
	*/
	static const TCHAR* const ToString(bool In);

	template<typename T>
	inline static FRotator Rotator(const T& InX = 0, const T& InY = 0, const T& InZ = 0) {
		return FRotator{ static_cast<double>(InX), static_cast<double>(InY), static_cast<double>(InZ) };
	}

	inline static FRotator Rotator(const FVector& InVector) {
		return FRotator{ InVector.X, InVector.Y, InVector.Z };
	}

	template<typename T, typename U> static void SetFlag(T& OutFlag, U InFlag, bool InValue) {
		if (InValue) {
			OutFlag = static_cast<T>(EInt(OutFlag) | EInt(InFlag));
		}
		else OutFlag = static_cast<T>(EInt(OutFlag) & ~EInt(InFlag));
	}

	template<typename T, typename U> static bool CheckFlag(T InFlag, U InArg) {
		return (static_cast<uint64>(InFlag) & static_cast<uint64>(InArg)) == static_cast<uint64>(InArg);
	}

	template<class E> static uint64 EInt(E InEnumValue) {
		return static_cast<uint64>(InEnumValue);
	}

	// 상수와 계수를 계산합니다
	// 반올림 필요 시 int로 캐스팅 합니다.
	template<typename T = float> static T CoefficientFormula(float InDefault, float InCoefficient, int InLevel) {
		// Base + Level * Percent Coefficient
		// 1렙 데미지 100 / 1렙당 데미지 5% 증가 / 스킬 레벨 2인 경우
		// 100 + 5 = 105
		return static_cast<T>(InDefault + InDefault * ((InLevel - 1) * InCoefficient * 0.01));
	}

	// 오류로 폐기
	//template<class E> static auto EInt(E InEnumValue) {
	//	return static_cast<std::make_unsigned_t<std::underlying_type_t<E>>>(InEnumValue);
	//}
	
	static bool IsIn(AActor* A, AActor* B, float InDistance);
	static bool IsIn(AActor* A, const FVector& B, float InDistance);
	static bool IsIn(const FVector& A, const FVector& B, float InDistance);
	static bool IsIn(const FVector& A, AActor* B, float InDistance);

	static FVector2d To2D(const FVector&);
	static FVector3d To3D(const FVector2d&);
	static FVector3d RemoveZ(const FVector&);
};