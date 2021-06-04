#include "ItemSlot.h"

#include "Single/GameInstance/RPGGameInst.h"

#include "Components/Image.h"

UItemSlot::UItemSlot(const FObjectInitializer& objectInitializer) :
	Super(objectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ITEM_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ItemInfo.DT_ItemInfo'"));
	if (DT_ITEM_INFO.Succeeded()) DT_ItemInfo = DT_ITEM_INFO.Object;


}

void UItemSlot::InitializeSlot(ESlotType slotType, FName inCode)
{
	Super::InitializeSlot(slotType, inCode);

	// ������ ���� ����
	SetItemInfo(inCode);

	// ������ �̹��� ����
	UpdateItemImage();
}

void UItemSlot::UpdateItemImage()
{
	UTexture2D* itemImage = nullptr;

	// ������ ������ ����ִٸ� ������ �̹����� ����մϴ�.
	if (ItemInfo.IsEmpty()) itemImage = T_Null;

	// ������ ������ ������� ���� ���
	else
	{
		// ������ �̹��� ��ΰ� ����ִٸ� ������ �̹��� ���
		if (ItemInfo.ItemImagePath.IsNull()) 
			itemImage = T_Null;

		// ������ �̹��� ��ΰ� ������� �ʴٸ� ������ �̹����� �ε��Ͽ� ����մϴ�.
		else itemImage = Cast<UTexture2D>(
				GetManager(FStreamableManager)->LoadSynchronous(ItemInfo.ItemImagePath));
		//FStreamableDelegate onLoadFin;
		//onLoadFin.BindLambda([]() { 
		//	GetSlotImage()->SetBrushFromTexture(itemImage);
		//	});
		//GetManager(FStreamableManager)->RequestAsyncLoad(ItemInfo.ItemImagePath, onLoadFin);
	}

	// ������ �̹��� ����
	GetSlotImage()->SetBrushFromTexture(itemImage);
}

void UItemSlot::SetItemInfo(FName itemCode)
{
	// ������ �ڵ尡 ����ִٸ�
	if (itemCode.IsNone())
	{
		// ������ ������ ���ϴ�.
		ItemInfo = FItemInfo();
		return;
	}

	// ������ ���� ã��
	FString contextString;
	FItemInfo* findedItemInfo = DT_ItemInfo->FindRow<FItemInfo>(itemCode, contextString);

	// ������ ������ ã�� ���ߴٸ� ������ ������ ���ϴ�.
	if (findedItemInfo == nullptr) ItemInfo = FItemInfo();
	else ItemInfo = *findedItemInfo;
}
