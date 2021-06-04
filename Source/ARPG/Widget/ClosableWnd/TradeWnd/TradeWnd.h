#pragma once

#include "CoreMinimal.h"
#include "Widget/ClosableWnd/ClosableWnd.h"

#include "Struct/ShopItemInfo/ShopItemInfo.h"

#include "Enum/TradeSeller.h"
#include "TradeWnd.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FTradeWndButtonSignature, class UTradeWnd *)

UCLASS()
class ARPG_API UTradeWnd : public UClosableWnd
{
	GENERATED_BODY()

public :
	// ���� / �Ǹ� ��ư�� Ŭ���Ǿ��� ��� ȣ��Ǵ� �븮��
	FTradeWndButtonSignature OnTradeButtonClickedEvent;

private :
	// ����� ���� ��ü�� ��Ÿ���ϴ�.
	class UItemSlot* ConnectedItemSlot;

	// �Ǹ��ڸ� ��Ÿ���ϴ�.
	ETradeSeller TradeSeller;

	// �Ǹ� ������ ����
	FShopItemInfo* ShopItemInfo;

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_ItemCost;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_TradeButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox * EditableTextBox_TradeCount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Result;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Trade;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Cancel;

protected :
	virtual void NativeConstruct() override;

public :
	// ��ȯ â�� �ʱ�ȭ�մϴ�.
	void InitializeTradeWnd(
		ETradeSeller tradeSeller,
		class UItemSlot* connectedItemSlot,
		FShopItemInfo* shopItemInfo = nullptr);

private  :
	UFUNCTION()
	void OnTradeButtonClicked();

	UFUNCTION()
	void OnTradeCountTextChanged(const FText& Text);

public :
	// �Է��� ���ڿ��� ����ִ��� Ȯ���մϴ�.
	bool IsInputTextEmpty() const;

	// �Է��� ������ int32 �������� ��ȯ�մϴ�.
	int32 GetInputTradeCount() const;

	FORCEINLINE class UItemSlot* GetConnectedItemSlot() const
	{ return ConnectedItemSlot; }

	FORCEINLINE FShopItemInfo* GetShopItemInfo() const
	{ return ShopItemInfo; }
};
