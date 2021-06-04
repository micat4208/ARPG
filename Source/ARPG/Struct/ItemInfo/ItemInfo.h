#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "Enum/ItemType.h"

#include "ItemInfo.generated.h"


// �⺻���� ������ ������ ��Ÿ���ϴ�.
USTRUCT(BlueprintType)
struct ARPG_API FItemInfo : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// ������ �ڵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "�⺻")
	FName ItemCode;

	// ������ Ÿ��
	// ��Ÿ, �Һ�, ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "�⺻")
	EItemType ItemType;

	// ������ �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "�⺻")
	FText ItemName;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "�⺻")
	FText ItemDescription;

	// ������ �̹��� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "�⺻")
	FSoftObjectPath ItemImagePath;

	// ���� �ִ� ����
	/// - ���Կ� ���� �� �ִ� �ִ� ������ ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "�⺻")
	int32 MaxSlotCount;

	// ������ �Ǹ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "�⺻")
	int32 Price;

public :
	FORCEINLINE bool IsEmpty() const
	{ return ItemCode.IsNone(); }

	FORCEINLINE bool operator==(FItemInfo& itemInfo)
	{ return (this->ItemCode == itemInfo.ItemCode); }

	FORCEINLINE bool operator!=(FItemInfo& itemInfo)
	{ return (this->ItemCode != itemInfo.ItemCode); }





};
