#include "PlayerInventoryItemSlot.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/ClosableWnd/PlayerEquipItemWnd/PlayerEquipItemWnd.h"
#include "Widget/BaseSlot/ItemSlot/PlayerEquipSlot/PlayerEquipSlot.h"

#include "Struct/EquipItemInfo/EquipItemInfo.h"

#include "Components/Image.h"

UPlayerInventoryItemSlot::UPlayerInventoryItemSlot(const FObjectInitializer& objInitializer) : 
	Super(objInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_EQUIP_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EquipItemInfo.DT_EquipItemInfo'"));
	if (DT_EQUIP_ITEM_INFO.Succeeded()) DT_EquipItemInfo = DT_EQUIP_ITEM_INFO.Object;
}

void UPlayerInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� Ÿ�� ����
	SlotType = ESlotType::ST_InventorySlot;

	// ���� �巡�� ���
	bAllowDragOperation = true;

	// ���� �巡���� ���۵Ǿ��� ���
	OnSlotDragStarted.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// ������ ����ִٸ� ���� ���
			if (GetItemInfo()->IsEmpty()) return;

			auto dragWidgetData = CreateSlotDragWidget();
			auto dragWidgetInstance = dragWidgetData.Get<0>();
			auto dragWidgetImage = dragWidgetData.Get<1>();

			// �巡�� �� ǥ�õǴ� ������ �����մϴ�.
			dragDropOp->DefaultDragVisual = dragWidgetInstance;

			// �巡�� �� ǥ�õǴ� �̹����� �����մϴ�.
			dragWidgetImage->SetBrushFromTexture(
				Cast<UTexture2D>(GetSlotImage()->Brush.GetResourceObject()));

			// ������ ������ �̹��� ������ �巡�� �������� �����մϴ�.
			SetSlotColorDragging();
		});

	// ���� �巡���� ��ҵǾ��� ���
	OnSlotDragCancelled.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			// ������ ����ִٸ� ���� ���
			if (GetItemInfo()->IsEmpty()) return;

			// ������ �̹��� ������ �⺻ �������� �����մϴ�.
			SetSlotColorNormal();
		} );

	// ���� �巡���� ���������� ������ ���
	OnSlotDragFinished.AddLambda(
		[this](USlotDragDropOperation* dragDropOp)
		{
			if (dragDropOp->OriginatedDragSlot->GetSlotType() == ESlotType::ST_InventorySlot)
			{
				auto originatedDragSlot = Cast<UPlayerInventoryItemSlot>(dragDropOp->OriginatedDragSlot);

				// ������ ����ִٸ� ���� ���
				if (originatedDragSlot->GetItemInfo()->IsEmpty()) return;

				// ������ �̹��� ������ �⺻ �������� �����մϴ�.
				dragDropOp->OriginatedDragSlot->SetSlotColorNormal();

				FPlayerCharacterInfo* playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();

				// �巡�׸� ���۽�Ų ���԰� ����� ��Ų ��ġ�� ���Կ� ��� �������� ������ �������� ��� �ִ����� ��Ÿ���ϴ�.
				bool bIsSameitem =
					playerInfo->InventoryItemInfos[originatedDragSlot->GetItemSlotIndex()].IsSameItem(
						playerInfo->InventoryItemInfos[GetItemSlotIndex()]);

				// ������ �������̶�� �������� ���� ��Ĩ�ϴ�.
				if (bIsSameitem)
					GetManager(UPlayerManager)->GetPlayerInventory()->MergeItem(originatedDragSlot, this);

				// ������ �������� �ƴ϶�� �κ��丮 ���� ����
				else GetManager(UPlayerManager)->GetPlayerInventory()->SwapItem(originatedDragSlot, this);
			}

			else if (dragDropOp->OriginatedDragSlot->GetSlotType() == ESlotType::ST_EquipSlot)
			{
				auto originatedDragSlot = Cast<UPlayerEquipSlot>(dragDropOp->OriginatedDragSlot);

				originatedDragSlot->SetSlotColorNormal();

				originatedDragSlot->DetachToEquipItemSlot(ItemSlotIndex);
			}
			
		});

	// ��Ŭ��, �� ���� Ŭ�� �� ��� �������� ������ŵ�ϴ�.
	OnMouseRightButtonClicked.AddLambda([this](ESlotType slotType) { AttachToEquipItemSlot(); });
	OnMouseLeftButtonDBClicked.AddLambda([this](ESlotType slotType) { AttachToEquipItemSlot(); });

}

void UPlayerInventoryItemSlot::InitializeItemSlot(
	ESlotType slotType, FName itemCode, int32 itemSlotIndex)
{
	Super::InitializeSlot(slotType, itemCode);

	ItemSlotIndex = itemSlotIndex;

	UpdateItemCountText();
}

void UPlayerInventoryItemSlot::UpdateInventoryItemSlot()
{
	UpdateItemImage();
	UpdateItemCountText();
}

void UPlayerInventoryItemSlot::AttachToEquipItemSlot(UPlayerEquipSlot* targetEquipSlot)
{
	// ���� �ش� ���Կ� �������� �������� �ʴ´ٸ� ���� X
	if (GetItemInfo()->IsEmpty()) return;

	// ���� �ش� ������ �������� ��� �������� �ƴ϶�� ���� X
	if (GetItemInfo()->ItemType != EItemType::Equipment) return;

	// PlayerInventory ��ü�� ����ϴ�.
	UPlayerInventory* playerInventory = GetManager(UPlayerManager)->GetPlayerInventory();

	// ���â�� ����ϴ�.
	UPlayerEquipItemWnd* playerEquipItemWnd = playerInventory->GetPlayerEquipItemWnd();

	// ��� ������ ������ ����ϴ�.
	FString contextString;
	FEquipItemInfo* equipItemInfo = DT_EquipItemInfo->FindRow<FEquipItemInfo>(
		GetItemInfo()->ItemCode, contextString);

	// ��� ������ ������ ã�� ���ߴٸ� ���� X
	if (equipItemInfo == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("UPlayerInventoryItemSlot.cpp :: %d LINE :: equipItemInfo is nullptr! (GetItemInfo()->ItemCode is %s)"),
			__LINE__, *GetItemInfo()->ItemCode.ToString());
		return;
	}

	// ��� ������ â�� ��ȿ�� ���
	if (IsValid(playerEquipItemWnd))
	{
		// targetEquipSlot �� ���޵��� �ʾҴٸ� ��� ������ â���� ������ ��ġ�ϴ� ������ ã���ϴ�.
		/// - (���� Ŭ���̳�, ��Ŭ������ ��� ������ �����ϴ� ���)
		if (targetEquipSlot == nullptr)
			targetEquipSlot = playerEquipItemWnd->GetPlayerEquipSlot(equipItemInfo->PartsType);

		// ������Ű���� ��� �������� ���� Ÿ���� ���� �� �ִ� ��� ������ ������ ã�� ���ߴٸ� ���� X
		if (targetEquipSlot == nullptr)
		{
			UE_LOG(LogTemp, Error,
				TEXT("UPlayerInventoryItemSlot.cpp :: %d LINE :: targetEquipSlot is nullptr (equipItemInfo->PartsType is %d)"),
				__LINE__, static_cast<uint8>(equipItemInfo->PartsType));

			return;
		}
		// ������Ű���� �������� Ÿ�԰� ��� ������ ���� Ÿ���� �ٸ� ���
		else if (equipItemInfo->PartsType != targetEquipSlot->GetPartsType())
			return;
	}

	// ������ �����ߴ� ������ ������ ������ ����
	FItemSlotInfo prevEquippedItemInfo;

	// ������ ����
	playerInventory->EquipItem(GetItemInfo()->ItemCode, &prevEquippedItemInfo);

	// �ش� ������ ���ϴ�.
	playerInventory->RemoveItem(GetItemSlotIndex());

	// ���â ����
	if (IsValid(playerEquipItemWnd))
		playerEquipItemWnd->UpdatePartsSlot();


	// ���� �����ߴ� �������� �κ��丮�� �߰��մϴ�.
	playerInventory->AddItem(prevEquippedItemInfo);
}

void UPlayerInventoryItemSlot::UpdateItemCountText()
{
	auto itemSlotInfo = GetManager(UPlayerManager)->GetPlayerInventory()->GetItemSlotInfo(ItemSlotIndex);

	SetSlotItemCount(itemSlotInfo->ItemCount);
}

