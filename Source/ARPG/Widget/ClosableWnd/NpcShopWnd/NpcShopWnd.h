#pragma once

#include "CoreMinimal.h"
#include "Widget/ClosableWnd/ClosableWnd.h"

#include "Struct/ShopInfo/ShopInfo.h"

#include "Enum/TradeSeller.h"
#include "NpcShopWnd.generated.h"

UCLASS()
class ARPG_API UNpcShopWnd final : 
	public UClosableWnd
{
	GENERATED_BODY()

private :
	TSubclassOf<class UShopItemWidget> BP_ShopItem;
	TSubclassOf<class UTradeWnd> BP_TradeWnd;

private :
	UPROPERTY()
	class UTradeWnd* TradeWnd;

private :
	UPROPERTY(meta = (BindWidget))
	class UGridPanel* GridPanel_ShopItems;

public :
	UNpcShopWnd(const FObjectInitializer& objInitializer);

protected :
	virtual void NativeConstruct() override;

private :
	// �Ǹ� �������� �߰��մϴ�.
	/// - shopItemInfo : �Ǹ� ������ ������ �����մϴ�.
	class UShopItemWidget* AddShopItemWidget(const FShopItemInfo& shopItemInfo);

public :
	// ���� â�� �ʱ�ȭ�մϴ�.
	/// - shopInfo : ���� ������ �����մϴ�.
	void InitializeNpcShop(FShopInfo* shopInfo);

	// �������� �Ǹ��մϴ�.
	void SaleItem(class UPlayerInventoryWnd* playerInventoryWnd, class UItemSlot* itemSlot);

	// ������ ��ȯ â�� �����մϴ�.
	class UTradeWnd* CreateTradeWnd(ETradeSeller tradeSeller, class UItemSlot * connectedItemSlot, FShopItemInfo * shopItemInfo = nullptr);

public :
	// �κ��丮 â�� ���� â ������ ���ϴ�.
	UFUNCTION()
	void FloatingInventoryWnd();
	
};
