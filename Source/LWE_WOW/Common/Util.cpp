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

