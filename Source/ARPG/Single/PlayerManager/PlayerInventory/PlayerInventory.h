#pragma once

#include "ARPG.h"
#include "UObject/NoExportTypes.h"

#include "Struct/ItemSlotInfo/ItemSlotInfo.h"

#include "Enum/InputModeType.h"
#include "Enum/PartsType.h"


#include "PlayerInventory.generated.h"

#ifndef ITEM_ALL
#define ITEM_ALL -1
#endif

UCLASS()
class ARPG_API UPlayerInventory : public UObject
{
	GENERATED_BODY()

private :
	TSubclassOf<class UPlayerInventoryWnd> BP_PlayerInventoryWnd;
	TSubclassOf<class UPlayerEquipItemWnd> BP_EquipItemWnd;
	class UDataTable* DT_EquipItemInfo;

private :
	UPROPERTY()
	class UPlayerInventoryWnd* PlayerInventoryWnd;

	UPROPERTY()
	class UPlayerEquipItemWnd * PlayerEquipItemWnd;


public :
	UPlayerInventory();

public :
	// �κ��丮 â�� �����մϴ�.
	class UPlayerInventoryWnd* CreateInventoryWnd(
		class UWidgetControllerWidget* widgetController,
		EInputModeType changeInputMode = EInputModeType::IM_Default, bool bShowCursor = true);

	// ��� ������ â�� �����մϴ�.
	class UPlayerEquipItemWnd* CreateEquipItemWnd(
		class UWidgetControllerWidget* widgetController,
		EInputModeType changeInputMode = EInputModeType::IM_Default, bool bShowCursor = true);

	// �κ��丮 â�� �ݽ��ϴ�.
	void CloseInventoryWnd();

	// ��� ������ â�� �ݽ��ϴ�.
	void CloseEquipItemWnd();

	// �κ��丮 â�� ����մϴ�.
	void ToggleInventoryWnd(class UWidgetControllerWidget* widgetController, bool bOpenEquipItemWnd = false);

	// ��� â�� ����մϴ�.
	void ToggleEquipItemWnd(class UWidgetControllerWidget* widgetController);

	// ������ ���� ������ ����ϴ�.
	FItemSlotInfo* GetItemSlotInfo(int32 slotIndex) const;

	// �κ��丮 �������� ��ü�մϴ�.
	void SwapItem(class UPlayerInventoryItemSlot* first, class UPlayerInventoryItemSlot* second);
	
	// �������� �κ��丮�� �߰��մϴ�.
	bool AddItem(FItemSlotInfo& newItemSlotInfo);

	// ������ �������� ��Ĩ�ϴ�.
	void MergeItem(class UPlayerInventoryItemSlot* ori, class UPlayerInventoryItemSlot* target);

	// �������� �κ��丮���� �����մϴ�.
	/// - itemSlotIndex : ������ ���� �ε����� �����մϴ�.
	/// - removeCount : ������ ������ �����մϴ�.
	///   ITEM_ALL �� ���޵ȴٸ� ��� �������� �����մϴ�.
	void RemoveItem(int32 itemSlotIndex, int32 removeCount = ITEM_ALL);

	// ��� �������� �����մϴ�.
	/// - equipItemCode : ������Ű���� ������ �ڵ带 �����մϴ�.
	/// - out_PrevItemSlotInfo : ������ ������Ų ������ ������ ���� ItemSlotInfo ������ �����մϴ�.
	void EquipItem(FName equipItemCode, FItemSlotInfo* out_PrevItemSlotInfo = nullptr);

	// ��� �������� ���� �����մϴ�.
	/// - partsType : ���� ������Ű���� ������ �����մϴ�.
	/// - out_ItemSlotInfo : ������ ������Ų ������ ������ ���� ItemSlotInfo ������ �����մϴ�.
	void DismountItem(EPartsType partsType, FItemSlotInfo* out_ItemSlotInfo = nullptr);

	// �÷��̾� ĳ������ ������� �����մϴ�.
	/// - ��� Mesh �� ���ŵ˴ϴ�.
	void UpdateCharacterVisual();

	// �÷��̾� ĳ���� ������� �����մϴ�.
	/// - updateParts : ���Ž�ų ������ �����մϴ�.
	/// - equipItemInfo : ��� ������ ������ �����մϴ�.
	///   - ���� nullptr �� �����Ѵٸ� �� �޼��� ������ ĳ���Ϳ� ������ ��� ������ �̿��Ͽ� �����͸� ��� �˴ϴ�.
	void UpdateCharacterVisual(EPartsType updateParts, struct FEquipItemInfo * equipItemInfo = nullptr);

public :
	FORCEINLINE class UPlayerInventoryWnd* GetPlayerInventoryWnd() const
	{ return PlayerInventoryWnd; }

	FORCEINLINE class UPlayerEquipItemWnd* GetPlayerEquipItemWnd() const
	{ return PlayerEquipItemWnd; }

};
