#pragma once

#include "CoreMinimal.h"

#include "Enum/CharacterClassType.h"
#include "Enum/PartsType.h"

#include "Struct/ItemSlotInfo/ItemSlotInfo.h"
#include "Struct/QuestInfo/QuestInfo.h"
#include "Struct/ProgressQuestInfo/ProgressQuestInfo.h"

#include "PlayerCharacterInfo.generated.h"

USTRUCT()
struct ARPG_API FPlayerCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public :
	// ĳ���� Ŭ���� Ÿ��
	UPROPERTY()
	ECharacterClassType CharacterClass;

	// ĳ���� ����
	UPROPERTY()
	int32 CharacterLevel;

	// �κ��丮 ���� ����
	UPROPERTY()
	int32 InventorySlotCount;

	// �������� ������ ����
	UPROPERTY()
	TArray<FItemSlotInfo> InventoryItemInfos;

	// �⺻ ���� ������ ����
	UPROPERTY()
	TMap<EPartsType, FItemSlotInfo> DefaultPartsInfos;

	// ���� ������ ����
	UPROPERTY()
	TMap<EPartsType, FItemSlotInfo> PartsInfos;

	// ������
	UPROPERTY()
	int32 Silver;

	// �������� ����Ʈ ���
	TArray<FProgressQuestInfo> ProgressQuestInfos;

	// �Ϸ��� ����Ʈ �ڵ� ���
	TArray<FName> CompletedQuestCodes;

public :
	FPlayerCharacterInfo();

	// �⺻ ��� �����۵� �ʱ�ȭ
	void InitializeDefaultEquipmentItems(
		TArray<FName>& defaultEquipmentItemCodes, const class UDataTable* dt_EquipItemInfo);

	// ��� ������ ������ ����ϴ�.
	/// - Return : <��� ������ ���� ����, ��ȯ�� ���� ������ �⺻ ��� ������ ������>
	TTuple<FItemSlotInfo, bool> GetEquippedItemSlotInfo(EPartsType partsType, bool* out_bReturnedAvailableData = nullptr) const;

	// ��� ��� ������ ������ ����ϴ�.
	TMap< EPartsType, TTuple<FItemSlotInfo, bool> > GetAllEquippedItemSlotInfo() const;

	// ���� ����Ʈ�� �߰��մϴ�.
	void AddProgressQuest(FName newQuestCode, FQuestInfo newQuestInfo);


};
