#pragma once

#include "CoreMinimal.h"

#include "Enum/PlayerBehavior.h"

#include "PlayerBehaviorData.generated.h"

using namespace EPlayerBehavior;

USTRUCT()
struct ARPG_API FPlayerBehaviorData
{
	GENERATED_USTRUCT_BODY()

public :
	// �ൿ Ÿ���� ��Ÿ���ϴ�.
	EPlayerBehavior::Type Behavior;

	// ������ �ڵ�, Npc �ڵ�, ���� �ڵ� ���� ��Ÿ���ϴ�.
	FName Code;

	// ������ ��Ÿ���ϴ�.
	int32 Count;

	// Ư���� ���ڿ��� ���޵˴ϴ�.
	FString StringData1;

public:
	FPlayerBehaviorData(EPlayerBehavior::Type behavior = BH_None);
	FPlayerBehaviorData(EPlayerBehavior::Type behavior, FString stringData1);
	FPlayerBehaviorData(EPlayerBehavior::Type behavior, FName code, int32 count);

public :
	// ������ ���� ����ϴ�.
	FORCEINLINE int32 GetItemCount() const
	{ return Count; }
};
