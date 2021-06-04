#include "PlayerInventoryWnd.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Actor/Controller/PlayerController/GamePlayerController/GamePlayerController.h"
#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Widget/BaseSlot/ItemSlot/PlayerInventoryItemSlot/PlayerInventoryItemSlot.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

#include "Component/ZoomableSpringArm/ZoomableSpringArmComponent.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/TextBlock.h"

UPlayerInventoryWnd::UPlayerInventoryWnd(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UPlayerInventoryItemSlot> BP_PLAYER_INVENTORY_ITEM_SLOT(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Slot/BP_PlayerInventoryItemSlot.BP_PlayerInventoryItemSlot_C'"));
	if (BP_PLAYER_INVENTORY_ITEM_SLOT.Succeeded()) BP_PlayerInventoryItemSlot = BP_PLAYER_INVENTORY_ITEM_SLOT.Class;
}

void UPlayerInventoryWnd::NativeConstruct()
{
	Super::NativeConstruct();

	WndSize = FVector2D(460.0f, 700.0f);

	InitializeInventoryWnd();

	// ������ ����
	UpdateSilver();

	for (auto itemSlot : ItemSlots)
	{
		auto info = GetManager(UPlayerManager)->GetPlayerInfo()->InventoryItemInfos[itemSlot->GetItemSlotIndex()];

		//LOG(TEXT("[%d] itemcode = %s || isEmpty = %d"), 
		//	itemSlot->GetItemSlotIndex(),
		//	*info.ItemCode.ToString(), info.IsEmpty());
	}
}

void UPlayerInventoryWnd::InitializeInventoryWnd()
{
#pragma region Create Inventory Slots

	// �÷��̾� ĳ���� ������ ����ϴ�.
	auto playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();

	// �κ��丮 ���� ������ ����ϴ�.
	int32 inventorySlotCount = playerInfo->InventorySlotCount;

	const int32 maxColumnCount = 6;
	int32 currentColumnCount = 0;

	// �κ��丮 ���Ե��� �����մϴ�.
	for (int32 i = 0; i < inventorySlotCount; ++i)
	{
		// �κ��丮 ������ �����մϴ�.
		auto newItemSlot = CreateItemSlot();

		// ���� �ʱ�ȭ
		newItemSlot->InitializeItemSlot(
			ESlotType::ST_InventorySlot,
			playerInfo->InventoryItemInfos[i].ItemCode, i);

		// ������ ������ �����մϴ�.
		UWidgetControllerWidget::SortGridPanelElem(
			newItemSlot, maxColumnCount, currentColumnCount);
	}
#pragma endregion

#pragma region Lock Camera Zoom
	// �κ��丮 â�� ������ ī�޶� ���� �����ϴ�.
	GetManager(UPlayerManager)->GetPlayerCharacter()->GetSpringArm()->SetUseZoom(false);

	// �κ��丮 â�� ������ ī�޶� ���� ����Ҽ� �ֵ��� �մϴ�.
	OnWndClosedEvent.AddLambda([this](UClosableWnd* closableWnd)
		{
			GetManager(UPlayerManager)->GetPlayerCharacter()->GetSpringArm()->SetUseZoom(true); 
		});
#pragma endregion

}

UPlayerInventoryItemSlot* UPlayerInventoryWnd::CreateItemSlot()
{
	// �κ��丮 ������ �����մϴ�.
	auto newItemSlot = CreateWidget<UPlayerInventoryItemSlot>(
		this, BP_PlayerInventoryItemSlot);

	ItemSlots.Add(newItemSlot);

	// GridPanel_ItemSlots �� �ڽ� �������� �߰��մϴ�.
	GridPanel_ItemSlots->AddChild(newItemSlot);

	// ������ �κ��丮 ������ ��ȯ�մϴ�.
	return newItemSlot;
}

void UPlayerInventoryWnd::UpdateInventoryItemSlots()
{
	FPlayerCharacterInfo* playerInfo = GetManager(UPlayerManager)->GetPlayerInfo();

	TArray<FItemSlotInfo>& inventoryIteminfos =
		playerInfo->InventoryItemInfos;

	for (int32 i = 0; i < ItemSlots.Num(); ++i)
	{
		auto itemSlot = ItemSlots[i];

		itemSlot->SetItemInfo(inventoryIteminfos[i].ItemCode);

		itemSlot->UpdateInventoryItemSlot();

		itemSlot->InitializeItemSlot(
			ESlotType::ST_InventorySlot,
			playerInfo->InventoryItemInfos[i].ItemCode, i);
	}
}

void UPlayerInventoryWnd::UpdateSilver()
{
	FText silverToText = FText::FromString(FString::FromInt(GetManager(UPlayerManager)->GetPlayerInfo()->Silver));
	Text_Silver->SetText(silverToText);
}
