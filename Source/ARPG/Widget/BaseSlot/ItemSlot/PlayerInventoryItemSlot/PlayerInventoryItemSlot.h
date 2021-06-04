#pragma once

#include "Widget/BaseSlot/ItemSlot/ItemSlot.h"
#include "PlayerInventoryItemSlot.generated.h"

UCLASS()
class ARPG_API UPlayerInventoryItemSlot : public UItemSlot
{
	GENERATED_BODY()
	
private :
	class UDataTable* DT_EquipItemInfo;

private :
	// �κ��丮 ���� �ε����� ��Ÿ���ϴ�.
	int32 ItemSlotIndex;


public :
	UPlayerInventoryItemSlot(const FObjectInitializer& objInitializer);

protected :
	virtual void NativeConstruct() override;

public :
	void InitializeItemSlot(ESlotType slotType, FName itemCode, int32 itemSlotIndex);

	// ������ ������ �����մϴ�.
	void UpdateInventoryItemSlot();

	// ��� �������� ������ŵ�ϴ�.
	void AttachToEquipItemSlot(class UPlayerEquipSlot* targetEquipSlot = nullptr);

private :
	// ������ ���� �ؽ�Ʈ�� �����մϴ�.
	void UpdateItemCountText();

public :
	FORCEINLINE int32 GetItemSlotIndex() const
	{ return ItemSlotIndex; }





};
