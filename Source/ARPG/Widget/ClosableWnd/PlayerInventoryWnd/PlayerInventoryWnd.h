#pragma once

#include "ARPG.h"
#include "Widget/ClosableWnd/ClosableWnd.h"
#include "PlayerInventoryWnd.generated.h"

UCLASS()
class ARPG_API UPlayerInventoryWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	TSubclassOf<class UPlayerInventoryItemSlot> BP_PlayerInventoryItemSlot;

private :
	TArray<class UPlayerInventoryItemSlot*> ItemSlots;

private :
	UPROPERTY(meta = (BindWidget))
	class UGridPanel* GridPanel_ItemSlots;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Silver;

public :
	UPlayerInventoryWnd(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	void InitializeInventoryWnd();

	// ������ ������ �����մϴ�.
	class UPlayerInventoryItemSlot* CreateItemSlot();

public :
	// �κ��丮 ������ ���Ե��� �����մϴ�.
	void UpdateInventoryItemSlots();

	// �������� �����մϴ�.
	void UpdateSilver();

public :
	TArray<class UPlayerInventoryItemSlot*>& GetItemSlots()
	{ return ItemSlots; }


	
};
