#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "Struct/NpcDialogInfo/NpcDialogInfo.h"

#include "NpcInfo.generated.h"

USTRUCT(BlueprintType)
struct ARPG_API FNpcInfo : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// Npc �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText NpcName;

	// �⺻ ��ȭ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNpcDialogInfo DefaultDialogInfo;

	// ���� �ڵ�
	UPROPERTY(EditAnywhere, BLueprintReadWrite)
	FName ShopCode;

	// ����Ʈ �ڵ�
	UPROPERTY(EditAnywhere, BLueprintReadWrite)
	TArray<FName> QuestCodes;

public :
	FORCEINLINE bool IsEmpty() const
	{ return NpcName.IsEmpty(); }

	FORCEINLINE bool IsShopUsing() const
	{ return !ShopCode.IsNone(); }

	FORCEINLINE bool IsQuestUsing() const
	{ return QuestCodes.Num() > 0; }
};
