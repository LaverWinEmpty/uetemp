// Fill out your copyright notice in the Description page of Project Settings.


#include "Util.h"

bool CUtil::Test(void* InObject, const TCHAR* const InTag, LogOption InFlag)
{
	return Test(InObject != nullptr, InTag, InFlag);
}

bool CUtil::Test(bool InValue, const TCHAR* const InTag, LogOption InFlag)
{
	if (InValue == false) {
		if ((InFlag & ON_FAILED)) {
			if ((InFlag & ON_FAILED_TO_CONSOLE)) WARNING(false, "[%s] is Null Pointer Exception", InTag ? InTag : _T("Unknown"));
			if ((InFlag & ON_FAILED_TO_DISPLAY)) ERROR  (true,  "[%s] is Null Pointer Exception", InTag ? InTag : _T("Unknown"));
		}
		return false;
	}

	else if ((InFlag & ON_SUCCEEDED)) {
		if ((InFlag & ON_SUCCEEDED_TO_CONSOLE)) WARNING(false, "[%s] is OK", InTag ? InTag : _T("Unknown"));
		if ((InFlag & ON_SUCCEEDED_TO_DISPLAY)) LOG    (true,  "[%s] is OK", InTag ? InTag : _T("Unknown"));
	}
	return true;
}

const TCHAR* const CUtil::ToString(bool InArg)
{
	return InArg ? _T("True") : _T("False");
}

bool CUtil::IsIn(AActor* A, AActor* B, float InDistance)
{
	return IsIn(A->GetActorLocation(), B->GetActorLocation(), InDistance);
}

bool CUtil::IsIn(AActor* A, const FVector& B, float InDistance)
{
	return IsIn(A->GetActorLocation(), B, InDistance);
}

bool CUtil::IsIn(const FVector& A, AActor* B, float InDistance)
{
	return IsIn(A, B->GetActorLocation(), InDistance);
}

bool CUtil::IsIn(const FVector& A, const FVector& B, float InDistance)
{
	return (A - B).SquaredLength() <= (InDistance * InDistance);
}

FVector2d CUtil::To2D(const FVector& V)
{
	return FVector2d{ V.X, V.Y };
}

FVector3d CUtil::To3D(const FVector2d& V)
{
	return FVector3d{ V.X, V.Y, 0 };
}

FVector3d CUtil::RemoveZ(const FVector& V)
{
	return FVector3d{ To2D(V), 0 };
}