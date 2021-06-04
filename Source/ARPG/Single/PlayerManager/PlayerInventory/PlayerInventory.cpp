#include "PlayerInventory.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"
#include "Struct/EquipItemInfo/EquipItemInfo.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"
#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Widget/BaseSlot/ItemSlot/PlayerInventoryItemSlot/PlayerInventoryItemSlot.h"

#include "Widget/ClosableWnd/PlayerInventoryWnd/PlayerInventoryWnd.h"
#include "Widget/ClosableWnd/PlayerEquipItemWnd/PlayerEquipItemWnd.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

#include "Component/PlayerBehaviorBroadcast/PlayerBehaviorBroadcastComponent.h"

#include "Components/CanvasPanelSlot.h"


UPlayerInventory::UPlayerInventory()
{
	static ConstructorHelpers::FClassFinder<UPlayerInventoryWnd> BP_PLAYER_INVENTORY_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/InventoryWnd/BP_PlayerInventoryWnd.BP_PlayerInventoryWnd_C'"));
	if (BP_PLAYER_INVENTORY_WND.Succeeded()) BP_PlayerInventoryWnd = BP_PLAYER_INVENTORY_WND.Class;

	static ConstructorHelpers::FClassFinder<UPlayerEquipItemWnd> BP_PLAYER_EQUIP_ITEM_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/EquipItemWnd/BP_EquipItemWnd.BP_EquipItemWnd_C'"));
	if (BP_PLAYER_EQUIP_ITEM_WND.Succeeded()) BP_EquipItemWnd = BP_PLAYER_EQUIP_ITEM_WND.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIP_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EquipItemInfo.DT_EquipItemInfo'"));
	if (DT_EQUIP_ITEM_INFO.Succeeded()) DT_EquipItemInfo = DT_EQUIP_ITEM_INFO.Object;
}

UPlayerInventoryWnd* UPlayerInventory::CreateInventoryWnd(UWidgetControllerWidget* widgetController, EInputModeType changeInputMode, bool bShowCursor)
{
	// �̹� �κ��丮 â�� ����� �ִٸ� ����� �κ��丮 â�� ��ȯ�մϴ�.
	if (IsValid(PlayerInventoryWnd)) return PlayerInventoryWnd;

	// �κ��丮 â�� �����մϴ�.
	PlayerInventoryWnd = Cast<UPlayerInventoryWnd>(
		widgetController->CreateWnd(BP_PlayerInventoryWnd, true, changeInputMode, bShowCursor));
	PlayerInventoryWnd->OnWndClosedEvent.AddLambda(
		[this](class UClosableWnd * closableWnd) 
		{ PlayerInventoryWnd = nullptr;});


	return PlayerInventoryWnd;
}

UPlayerEquipItemWnd* UPlayerInventory::CreateEquipItemWnd(
	UWidgetControllerWidget* widgetController, EInputModeType changeInputMode, bool bShowCursor)
{
	if (IsValid(PlayerEquipItemWnd)) return PlayerEquipItemWnd;

	// â�� ���ϴ�.
	PlayerEquipItemWnd = Cast<UPlayerEquipItemWnd>(
		widgetController->CreateWnd(BP_EquipItemWnd, true, changeInputMode, bShowCursor));

	// â ���� �̺�Ʈ �߰�
	PlayerEquipItemWnd->OnWndClosedEvent.AddLambda([this](class UClosableWnd* closableWnd)
		{ PlayerEquipItemWnd = nullptr; });

	// �κ��丮 â�� �����ִٸ�
	if (IsValid(PlayerInventoryWnd))
	{
		// �κ��丮 ���� ũ�⸦ �����մϴ�.
		FVector2D inventoryHalfSize = PlayerInventoryWnd->GetWndSize() * 0.5f;

		// �κ��丮 ��ġ�� �����մϴ�.
		FVector2D inventoryWndPos = PlayerInventoryWnd->GetCanvasPanelSlot()->GetPosition();

		// ���â ���� ũ�⸦ �����մϴ�.
		FVector2D equipItemWndHalfSize = PlayerEquipItemWnd->GetWndSize() * 0.5f;

		// ���â�� ��ġ�� ����մϴ�.
		FVector2D equipItemWndPos =
			inventoryWndPos + (inventoryHalfSize + equipItemWndHalfSize) * FVector2D(-1.0f, 0.0f);

		equipItemWndPos.Y += equipItemWndHalfSize.Y - inventoryHalfSize.Y;
		//equipItemWndPos.Y += inventoryHalfSize.Y - equipItemWndHalfSize.Y;

		// ��� â�� ��ġ�� �����մϴ�.
		PlayerEquipItemWnd->GetCanvasPanelSlot()->SetPosition(equipItemWndPos);
	}



	return PlayerEquipItemWnd;
}

void UPlayerInventory::CloseInventoryWnd()
{
	if (IsValid(PlayerInventoryWnd))
	{
		PlayerInventoryWnd->CloseThisWnd();
		PlayerInventoryWnd = nullptr;
	}
}

void UPlayerInventory::CloseEquipItemWnd()
{
	if (IsValid(PlayerEquipItemWnd))
	{
		PlayerEquipItemWnd->CloseThisWnd();
		PlayerEquipItemWnd = nullptr;
	}
}

void UPlayerInventory::ToggleInventoryWnd(UWidgetControllerWidget* widgetController, bool bOpenEquipItemWnd)
{
	if (IsValid(PlayerInventoryWnd))
	{
		CloseInventoryWnd();
		CloseEquipItemWnd();
	}
	else
	{
		CreateInventoryWnd(widgetController, EInputModeType::IM_GameAndUI, true);

		if (bOpenEquipItemWnd)
			CreateEquipItemWnd(widgetController, EInputModeType::IM_GameAndUI, true);

	}
}

void UPlayerInventory::ToggleEquipItemWnd(UWidgetControllerWidget* widgetController)
{
	if (IsValid(PlayerEquipItemWnd))
		CloseEquipItemWnd();
	else CreateEquipItemWnd(widgetController, EInputModeType::IM_GameAndUI, true);
}

FItemSlotInfo* UPlayerInventory::GetItemSlotInfo(int32 slotIndex) const
{
	return &GetManager(UPlayerManager)->GetPlayerInfo()->InventoryItemInfos[slotIndex];
}

void UPlayerInventory::SwapItem(UPlayerInventoryItemSlot* first, UPlayerInventoryItemSlot* second)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	TArray<FItemSlotInfo>& inventoryItemInfo = playerInfo->InventoryItemInfos;

	auto tempItemInfo = inventoryItemInfo[first->GetItemSlotIndex()];

	inventoryItemInfo[first->GetItemSlotIndex()] = inventoryItemInfo[second->GetItemSlotIndex()];
	inventoryItemInfo[second->GetItemSlotIndex()] = tempItemInfo;

	first->SetItemInfo(inventoryItemInfo[first->GetItemSlotIndex()].ItemCode);
	second->SetItemInfo(inventoryItemInfo[second->GetItemSlotIndex()].ItemCode);

	first->UpdateInventoryItemSlot();
	second->UpdateInventoryItemSlot();
}

bool UPlayerInventory::AddItem(FItemSlotInfo& newItemSlotInfo)
{
	Action(fnFillSlot, (FItemSlotInfo&, TArray<FItemSlotInfo>&, int32)) =
		[](FItemSlotInfo& newItemSlotInfo, TArray<FItemSlotInfo>& inventoryItemInfos, int32 slotIndex)
	{
		// �������� �߰��� �� �ִ� ���� ������ �����Ѵٸ�
		int32 addableItemCount = inventoryItemInfos[slotIndex].MaxSlotCount - inventoryItemInfos[slotIndex].ItemCount;
		if (addableItemCount > 0)
		{
			// �߰��� �� �ִ� ���� ������ �Ųٸ�, �������� �ִ��� ä��ϴ�.
			for (int32 i = 0;
				((i < addableItemCount) && newItemSlotInfo.ItemCount > 0);
				++i)
			{
				// �������� �߰��մϴ�.
				++inventoryItemInfos[slotIndex].ItemCount;

				// �߰��� �������� �����մϴ�.
				--newItemSlotInfo.ItemCount;
			}
		}
	};


	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	TArray<FItemSlotInfo>& inventoryItemInfos = playerInfo->InventoryItemInfos;

	// �ൿ �̺�Ʈ ���� ��ü
	auto& playerBehaviorBroadcast = *GetManager(UPlayerManager)->GetPlayerController()->GetPlayerBehaviorBroadcast();

	// �߰��� ������ ������ �����մϴ�.
	/// - �ൿ �̺�Ʈ�� ���� ���˴ϴ�.
	int32 prevItemCount = newItemSlotInfo.ItemCount;

	// �߰��� ������ ������ 0 �̶�� ���� X
	if (newItemSlotInfo.ItemCount <= 0)
	{
		return true;
	}

	for (int32 i = 0; i < playerInfo->InventorySlotCount; ++i)
	{
		// ���� �߰��Ϸ��� �����۰� ������ �������� ���� ������ ã�Ҵٸ�
		if (inventoryItemInfos[i].IsSameItem(newItemSlotInfo))
		{
			// ������ ä���
			fnFillSlot(newItemSlotInfo, inventoryItemInfos, i);

			// �κ��丮 â�� �����ִٸ� �����մϴ�.
			if (IsValid(PlayerInventoryWnd))
				PlayerInventoryWnd->UpdateInventoryItemSlots();
		}

		// �� ������ ������ ã�Ҵٸ�
		else if (inventoryItemInfos[i].IsEmpty())
		{
			inventoryItemInfos[i] = newItemSlotInfo;
			inventoryItemInfos[i].ItemCount = 0;

			// ������ ä���
			fnFillSlot(newItemSlotInfo, inventoryItemInfos, i);

			// �κ��丮 â�� �����ִٸ� �����մϴ�.
			if (IsValid(PlayerInventoryWnd))
				PlayerInventoryWnd->UpdateInventoryItemSlots();
		}

		// ��� �������� �߰��ߴٸ�
		if (newItemSlotInfo.ItemCount == 0)
		{
			// BH_GetItem [�κ��丮�� ��� ������ �߰� ��]
			FPlayerBehaviorData bhData(BH_GetItem, newItemSlotInfo.ItemCode, prevItemCount);
			playerBehaviorBroadcast.AddBehavior(bhData);
			return true;
		}
	}

	// �������� �ϳ��� �߰��ߴٸ�
	if (prevItemCount != newItemSlotInfo.ItemCount)
	{
		// �߰��� ������ ������ ����ϴ�.
		int32 addedItemCount = prevItemCount - newItemSlotInfo.ItemCount;

		// BH_GetItem [�κ��丮�� ������ �߰� ��]
		FPlayerBehaviorData bhData(BH_GetItem, newItemSlotInfo.ItemCode, addedItemCount);
		playerBehaviorBroadcast.AddBehavior(bhData);
	}


	return false;
}

void UPlayerInventory::MergeItem(UPlayerInventoryItemSlot* ori, UPlayerInventoryItemSlot* target)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();

	FItemSlotInfo& oriItemSlotInfo = playerInfo->InventoryItemInfos[ori->GetItemSlotIndex()];
	FItemSlotInfo& targetItemSlotInfo = playerInfo->InventoryItemInfos[target->GetItemSlotIndex()];

	// ���Կ� �� �� �ִ� �ִ� ������ ����
	int32 maxSlotCount = ori->GetItemInfo()->MaxSlotCount;

	// �� �� �ϳ��� �ִ� ������� ������ �Ͼ���� �մϴ�.
	if (oriItemSlotInfo.ItemCount == maxSlotCount ||
		targetItemSlotInfo.ItemCount == maxSlotCount)
		SwapItem(ori, target);
	else
	{
		// �߰� ������ ������ ������ ����մϴ�.
		int32 addable = maxSlotCount - targetItemSlotInfo.ItemCount;

		// �ű���� �������� ������ ������ ������ �������� ū ���
		if (addable > oriItemSlotInfo.ItemCount)
			// �ű� ������ ���� �ƾ��� ������ �����մϴ�.
			addable = oriItemSlotInfo.ItemCount;

		// �������� �ű�ϴ�.
		oriItemSlotInfo.ItemCount -= addable;
		targetItemSlotInfo.ItemCount += addable;

		// �ű� �� ������ ����ִٸ�
		if (oriItemSlotInfo.ItemCount == 0)
		{
			oriItemSlotInfo.Clear();
			ori->SetItemInfo(FName());
		}

		// ���� ����
		ori->UpdateInventoryItemSlot();
		target->UpdateInventoryItemSlot();
	}
}

void UPlayerInventory::RemoveItem(int32 itemSlotIndex, int32 removeCount)
{
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();
	auto playerInventoryWnd = PlayerInventoryWnd;

	TArray<FItemSlotInfo>& inventoryItemInfo = playerInfo->InventoryItemInfos;

	// ������ ������ �������� removeCount ��ŭ ���ϴ�.
	/// - removeCount �� ITEM_ALL �� ���޵Ǿ��ٸ� ������ ���ϴ�.
	inventoryItemInfo[itemSlotIndex].ItemCount -= 
		(removeCount == ITEM_ALL) ? 
		inventoryItemInfo[itemSlotIndex].ItemCount : 
		removeCount;

	// ������ ������ 0 ���϶�� ������ ���ϴ�.
	if (inventoryItemInfo[itemSlotIndex].ItemCount <= 0)
		inventoryItemInfo[itemSlotIndex].Clear();

	// �κ��丮 â�� �����ִٸ�
	if (IsValid(playerInventoryWnd))
		// �κ��丮 â�� ������ ���Ե��� �����մϴ�.
		playerInventoryWnd->UpdateInventoryItemSlots();
}

void UPlayerInventory::EquipItem(FName equipItemCode, FItemSlotInfo* out_PrevItemSlotInfo)
{
	// ��� ������ ������ ����ϴ�.
	FString contextString;
	FEquipItemInfo* equipItemInfo = DT_EquipItemInfo->FindRow<FEquipItemInfo>(equipItemCode, contextString);

	if (equipItemInfo == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory.cpp :: %d LINE :: equipItemCode(%s) is not valid!"),
			__LINE__, *equipItemCode.ToString());
		return;
	}

	// ���� ������ ������ ���޹��� ������ ���޵Ǿ��ٸ�
	if (out_PrevItemSlotInfo)
	{
		// ������ ������ ������ ������ ����ϴ�.
		bool bReturnedAvailableData;
		auto prevEquipItemData = GetManager(UPlayerManager)->GetPlayerInfo()->GetEquippedItemSlotInfo(
			equipItemInfo->PartsType, &bReturnedAvailableData);

		// ������ ������ ��� �������� �ʴ´ٸ�
		/// - �⺻ ����� ������ ������ ��� �������� ����
		if (prevEquipItemData.Get<1>())
			// �� ������ ��ȯ��ŵ�ϴ�.
			*out_PrevItemSlotInfo = FItemSlotInfo();

		// ������ ������ ��� �����Ѵٸ�, ��� ������ ��ȯ�մϴ�.
		else *out_PrevItemSlotInfo = prevEquipItemData.Get<0>();
	}

	// ��� ������ ����
	GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos[equipItemInfo->PartsType] =
		FItemSlotInfo(equipItemCode, 1, 1);

	// ����� ����
	UpdateCharacterVisual(equipItemInfo->PartsType, equipItemInfo);
}

void UPlayerInventory::DismountItem(EPartsType partsType, FItemSlotInfo* out_ItemSlotInfo)
{
	// ������ ������ ������ ������ ����ϴ�.
	auto prevEquipitemData = GetManager(UPlayerManager)->GetPlayerInfo()->GetEquippedItemSlotInfo(partsType);

	// ������ ������ ������ ������ ���� ������ ���޵Ǿ��ٸ�
	if (out_ItemSlotInfo)
	{
		// ������ ������ ��� �������� �ʴ´ٸ�
		if (prevEquipitemData.Get<1>())
			*out_ItemSlotInfo = FItemSlotInfo();

		// ������ ������ ��� �����Ѵٸ�
		else *out_ItemSlotInfo = prevEquipitemData.Get<0>();
	}

	// ��� ������ ����
	GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos[partsType] = FItemSlotInfo();

	// ����� ����
	UpdateCharacterVisual(partsType);

}

void UPlayerInventory::UpdateCharacterVisual()
{
	// ���� ������ ��� ���� Ÿ���� ����ϴ�.
	TArray<EPartsType> equipItemPartsTypes;
	GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos.GetKeys(equipItemPartsTypes);
	/// - TMap.GetKeys(TArray<KeyType>* outKeys) : TMap ����� ��� Ű�� �迭�� ��ȯ�մϴ�.
	/// - outKeys : ��¿� �Ű� �����̸�, TMap ��ҵ��� Ű�� ������ �迭�� �����մϴ�.
	
	// ����� ����
	for (auto partsType : equipItemPartsTypes)
		UpdateCharacterVisual(partsType);
}

void UPlayerInventory::UpdateCharacterVisual(EPartsType updateParts, FEquipItemInfo* equipItemInfo)
{
	// �÷��̾� ĳ���� ����
	FPlayerCharacterInfo* playerCharacterInfo = GetManager(UPlayerManager)->GetPlayerInfo();

	// ���� ������ ������ updateParts �� ��ġ�ϴ� ���� ������ ����ϴ�.
	bool bReturnedAvailableData = false; // ��ȯ�� �����Ͱ� ��ȿ�� ���������� Ȯ���ϱ� ���� ����
	TTuple<FItemSlotInfo, bool> equippedPartsData = playerCharacterInfo->GetEquippedItemSlotInfo(
		updateParts, &bReturnedAvailableData);

	// ��ȿ�� �����Ͱ� �ƴ϶�� ����
	if (!bReturnedAvailableData) return;

	// ���� ������ ��� ������ ���� ����
	FItemSlotInfo* equippedItemSlotInfo = &equippedPartsData.Get<0>();

	// ��ȿ�� ������������, ������ ����ִٸ� ����
	if (equippedItemSlotInfo->IsEmpty()) return;

	// �÷��̾� ĳ���� ����
	APlayerCharacter* playerCharacter = GetManager(UPlayerManager)->GetPlayerCharacter();

	// �÷��̾� ĳ���Ͱ� ��ȿ���� �ʴٸ� �޼��� ����
	if (!IsValid(playerCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory.cpp :: %d LINE :: playerCharacter is not valid!"), __LINE__);
		return;
	}

	// ��� ������ ������ ����ϴ�.
	if (equipItemInfo == nullptr)
	{
		FString contextString;
		equipItemInfo = DT_EquipItemInfo->FindRow<FEquipItemInfo>(equippedItemSlotInfo->ItemCode, contextString);
	}

	// Skeletal Mesh �ּ� �ε�
	USkeletalMesh* loadedMesh = Cast<USkeletalMesh>(
		GetManager(FStreamableManager)->LoadSynchronous(equipItemInfo->MeshPath));

	USkeletalMesh* loadedSetMesh = nullptr;
	if (!equipItemInfo->SetMeshPath.IsNull())
	{
		loadedSetMesh = Cast<USkeletalMesh>(
			GetManager(FStreamableManager)->LoadSynchronous(equipItemInfo->SetMeshPath));
	}


	if (!IsValid(loadedMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory.cpp :: %d LINE :: mesh path(%s) is not valid"),
			__LINE__, *equipItemInfo->MeshPath.ToString());
		return;
	}
	
	// ������ �������� Mesh ����
	if (equipItemInfo->PartsType == EPartsType::PT_Glove)
	{
		playerCharacter->GetParts()[EPartsType::PT_RGlove]->SetSkeletalMesh(loadedMesh);
		playerCharacter->GetParts()[EPartsType::PT_LGlove]->SetSkeletalMesh(loadedSetMesh);
	}
	else playerCharacter->GetParts()[equipItemInfo->PartsType]->SetSkeletalMesh(loadedMesh);
}
