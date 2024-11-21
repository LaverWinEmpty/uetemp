#pragma once

#include "CoreMinimal.h"

enum EActionID {
#include "ActionID"
};

enum EModFlag {
	MOD_CTRL_L  = 0x001,
	MOD_CTRL_R  = 0x010,
	MOD_SHIFT_L = 0x002,
	MOD_SHIFT_R = 0x020,
	MOD_ALT_L   = 0x004,
	MOD_ALT_R   = 0x040,
	MOD_MOUSE_L = 0x100,
	MOD_MOUSE_R = 0x200,
};

// 타겟의 유형
// NPC, Player 구분없이 행동 가능한 경우를 위해 플래그로 정의
UENUM(BlueprintType)
enum class ETargetType : uint8 {
	// false: NPC, 적, 소환수 등
	NPC = 0 UMETA(DisplayName = "NPC"),    
	// true:  플레이어
	PLAYER = 1 UMETA(DisplayName = "Player"), 
};
ENUM_CLASS_FLAGS(ETargetType)

// 팀 ID
// 중립의 경우 A | B 가능하기 때문에 플래그로 정의
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETeamType : uint8 {
	// 플레이어 진영 상관 없이 적대
	NONE  = 0 UMETA(Hidden),                   
	// 적
	ENENMY = 1 << 0 UMETA(DisplayName = "Enemy"),    
	// 진영 A
	A = 1 << 1 UMETA(DisplayName = "Alliance"), 
	// 진영 B
	B = 1 << 2 UMETA(DisplayName = "Horde"),    
};
ENUM_CLASS_FLAGS(ETeamType)

// 타겟과의 관계 플래그
// 스킬에서 사용할 수 있도록 플래그로 정의
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ERelationType : uint8 {
	// NPC 등
	NONE = 0 UMETA(Hidden),              
	// 자신
	SELF = 1 << 0 UMETA(DisplayName = "Self"),
	// 팀
	HARM = 1 << 1 UMETA(DisplayName = "Harm"),
	// 적
	HELP = 1 << 2 UMETA(DisplayName = "Help"),
};
ENUM_CLASS_FLAGS(ERelationType);

// 직업 코드
// 여러 직업이 배울 수 / 사용할 수 있는 스킬을 위해 플래그로 정의
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EClassCode : uint8 {
	NONE      = 0      UMETA(Hidden),
	BARBARIAN = 1 << 0 UMETA(DisplayName = "Barbarian"),
	DWARF     = 1 << 1 UMETA(DisplayName = "Dwarf"),
	KNIGHT    = 1 << 2 UMETA(DisplayName = "Knight"),
	WARRIOR   = 1 << 3 UMETA(DisplayName = "Warrior"),

	TESTER = 255
};
ENUM_CLASS_FLAGS(EClassCode);

// 무기 코드
// 언젠간 쓰겠지 모션같은거 ㅁㄹ
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EWeaponCode : uint8 {
	NONE = 0 UMETA(Hidden),
};
ENUM_CLASS_FLAGS(EWeaponCode);

// 아이템 메시 유형
UENUM(BlueprintType)
enum class EItemMeshType : uint8 {
	CONSUM UMETA(DisplayName = "No Mesh Type (Consum Item)"),
	WEAPON UMETA(DisplayName = "Static Mesh Type (Weapon)"),
	ARMOR  UMETA(DisplayName = "Skeletal Mesh Type (Armor)"),

	NONE     = CONSUM UMETA(Hidden),
	STATIC   = WEAPON UMETA(Hidden),
	SKELETAL = ARMOR  UMETA(Hidden),
};

// 아이템 슬롯 칸 Indexable
UENUM(BlueprintType)
enum class EItemSlotTypeIdx : uint8 {
	NONE   UMETA(DisplayName = "Can not euqipment"),
	WEAPON UMETA(DisplayName = "Weapon"),
};

#define ECC_ACTOR_FINDER ECollisionChannel::ECC_GameTraceChannel1 // 캐릭터 찾는 데만 사용