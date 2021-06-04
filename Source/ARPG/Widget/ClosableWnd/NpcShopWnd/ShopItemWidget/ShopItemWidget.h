#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Struct/ShopItemInfo/ShopItemInfo.h"

#include "ShopItemWidget.generated.h"

UCLASS()
class ARPG_API UShopItemWidget final : 
public UUserWidget
{
	GENERATED_BODY()

private :
	class UDataTable* DT_ItemInfo;

private :
	// Npc ���� â ��ü�� ��Ÿ���ϴ�.
	class UNpcShopWnd* NpcShopWnd;

	// ���� ������ ����
	FShopItemInfo ShopItemInfo;

private :
	UPROPERTY(meta = (BindWidget))
	class UItemSlot * BP_ItemSlot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Cost;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_ItemName;

public :
	UShopItemWidget(const FObjectInitializer& ObjectInitializer);

public :
	void InitializeShopItemWidget(class UNpcShopWnd* npcShopWnd, FShopItemInfo shopItemInfo);

protected :
	// ���콺 ���� Ŭ�� �Է��� ���� ��� ȣ��˴ϴ�.
	virtual FReply NativeOnMouseButtonDoubleClick(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	/// - InGeometry : ������ ��ġ, ũ�⸦ ��Ÿ���ϴ�.
	/// - InMouseEvent : �Է� �̺�Ʈ ����
	
private :
	// �������� �����մϴ�.
	void BuyItem();
};
