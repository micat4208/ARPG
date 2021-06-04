#pragma once

#include "Widget/BaseSlot/BaseSlot.h"
#include "Struct/ItemInfo/ItemInfo.h"
#include "ItemSlot.generated.h"

UCLASS()
class ARPG_API UItemSlot : 
	public UBaseSlot
{
	GENERATED_BODY()

private :
	class UDataTable* DT_ItemInfo;

private :
	FItemInfo ItemInfo;

public :
	UItemSlot(const FObjectInitializer& objectInitializer);

public :
	virtual void InitializeSlot(ESlotType slotType, FName inCode) override;

	// ������ �̹����� �����մϴ�.
	void UpdateItemImage();

public :
	void SetItemInfo(FName itemCode);

	FORCEINLINE FItemInfo* GetItemInfo()
	{ return &ItemInfo; }


	
};
