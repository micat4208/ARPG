#pragma once

#include "CoreMinimal.h"
#include "ItemSlotInfo.generated.h"

USTRUCT()
struct ARPG_API FItemSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// ������ �ڵ�
	UPROPERTY()
	FName ItemCode;

	// ������ ����
	UPROPERTY()
	int32 ItemCount;

	// ���Կ� ����� �� �ִ� �ִ� ����
	UPROPERTY()
	int32 MaxSlotCount;

public :
	FItemSlotInfo();
	FItemSlotInfo(FName itemCode, int32 itemCount = 0, int32 maxSlotCount = 0);

	FORCEINLINE bool operator==(const FItemSlotInfo& itemSlotInfo)
	{
		return
			this->ItemCode == itemSlotInfo.ItemCode &&
			this->ItemCount == itemSlotInfo.ItemCount;
	}

	FORCEINLINE bool operator!=(const FItemSlotInfo& itemSlotInfo)
	{
		return
			this->ItemCode != itemSlotInfo.ItemCode ||
			this->ItemCount != itemSlotInfo.ItemCount;
	}

	// ���� ���������� Ȯ���մϴ�.
	FORCEINLINE bool IsSameItem(const FItemSlotInfo& itemSlotInfo) const
	{
		if (itemSlotInfo.IsEmpty()) return false;

		return (ItemCode == itemSlotInfo.ItemCode);
	}


	// �ش� �����Ͱ� ����ִ��� Ȯ���մϴ�.
	FORCEINLINE bool IsEmpty() const
	{ return ItemCode.IsNone(); }

	// �ش� �����͸� ���ϴ�.
	FORCEINLINE void Clear()
	{ ItemCode = TEXT(""); }

};
