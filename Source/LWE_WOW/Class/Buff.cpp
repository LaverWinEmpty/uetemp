#include "Buff.h"

#include <LWE_WOW/Generic/GenericCharacter.h>


CBuff::CBuff(AGenericCharacter* InParent) : m_Parent(InParent)
{
}

CBuff::~CBuff()
{
}

void CBuff::Add(UBuffData* InData, int InLevel)
{
    TSharedPtr<Applied> Ptr = nullptr;

    // 이미 적용중이라면
    if (m_Applied.Find(InData->Name)) {
        Ptr = m_Applied[InData->Name]; // 가져옵니다
        
        // 더 강한 효과 유지중이라면 무시
        // 안내는 추후 처리
        if (Ptr->Level > InLevel) {
            return;
        }

        // 중첩 가능 횟수를 검사합니다.
        if (Ptr->State.Overlapped < Ptr->Info.Overlapped) {
            ++Ptr->State.Overlapped; // 중첩 수를 증가시킵니다.
        }
        Ptr->State.Duration = 0; // 현재 지난 시간을 초기화 합니다.
    }

    // 없는 효과면 새롭게 적용합니다.
    else {
        Ptr = MakeShared<Applied>();
        Ptr->Info = InData->Calculate(InLevel);
        Ptr->State = InData->Initialize();

        m_Applied.Add(InData->Name, Ptr); // 추가합니다.
    }

    InData->OnBegin(m_Parent);
    if (InData->Option & static_cast<uint8>(EBuffFlag::EALRY_TICK)) {
        InData->OnTick(m_Parent); // 플래그 켜져있으면 즉시 시전
    }
	Ptr->Data = InData;
}

void CBuff::OnTick(float DeltaTime)
{
    for (TMap<FName, TSharedPtr<Applied>>::TIterator Itr(m_Applied); Itr; ++Itr)
    {
        auto Buff = Itr.Value();

        if (Buff->Data->Option & static_cast<uint8>(EBuffFlag::SKIP_TICK)) {
            continue;
        }

        if (m_Parent->IsDead) {
            // 죽어도 버프 유지
            if ((Buff->Data->Option & static_cast<uint8>(EBuffFlag::KEEP_WHEN_DEAD)) == 0) {
                Itr.RemoveCurrent();
            }
            continue;
        }

        // 간격 시간이 됐는지 확인합니다
        Buff->State.Interval += DeltaTime;
        
        // 시간이 됐을 때 캐릭터 살아있으면
        if (!m_Parent->IsDead && Buff->State.Interval >= Buff->Info.Interval) {
            Buff->State.Interval -= Buff->Info.Interval; // 다시 카운트 하기 위해 뺍니다
            Buff->State.Duration += Buff->Info.Interval; // 정확하게 계산하기 위해 Duration에 같은 값을 더합니다.
            Buff->Data->OnTick(m_Parent);            // 실행됩니다.

            if (m_Parent->IsDead) {
                // 죽어도 버프 유지
                if ((Buff->Data->Option & static_cast<uint8>(EBuffFlag::KEEP_WHEN_DEAD)) == 0) {
                    Itr.RemoveCurrent();
                }
                continue;
            }
        }

        // 영구 버프면 종료
        if (Buff->Data->Option & static_cast<uint8>(EBuffFlag::INFINITE)) {
            continue;
        }

        // 시간이 지난 경우를 검사합니다.
        // 지속 시간 + 현재 시간도 지속시간을 넘겼는지 검사합니다.
        // Dutation + Interval 하는 이유는
        // 지속시간 3초, 간격 2초인 경우
        // Duation 2 + Interval 1 로 계산해야 하기 때문입니다.
        // 일회성 스킬 검사의 경우는 Duration이 0이기 때문에 필요 없습니다.
        if (Buff->State.Duration >= Buff->Info.Duration ||
            (Buff->State.Duration + Buff->State.Interval) >= Buff->Info.Duration) {
            Buff->Data->OnEnd(m_Parent); // 종료 시 함수를 호출해줍니다.
            Itr.RemoveCurrent();                 // 종료
        }
    }
}
