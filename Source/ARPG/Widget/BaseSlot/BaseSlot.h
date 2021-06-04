#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"
#include "Enum/SlotType.h"

#include "Widget/BaseSlot/SlotDragDropOperation/SlotDragDropOperation.h"

#include "BaseSlot.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSlotMouseEventSignature, ESlotType)
DECLARE_MULTICAST_DELEGATE_OneParam(FSlotDragEventSignature, class USlotDragDropOperation*)

UCLASS()
class ARPG_API UBaseSlot : public UUserWidget
{
	GENERATED_BODY()

public :
	// ���콺 ���� ��ư Ŭ�� �� ȣ��Ǵ� �븮��
	FSlotMouseEventSignature OnMouseLeftButtonClicked;

	// ���콺 ���� ��ư ���� Ŭ�� �� ȣ��Ǵ� �븮��
	FSlotMouseEventSignature OnMouseLeftButtonDBClicked;
	
	// ���콺 ������ ��ư Ŭ�� �� ȣ��Ǵ� �븮��
	FSlotMouseEventSignature OnMouseRightButtonClicked;
	
	// ���� �巡�� ���� �� ȣ��Ǵ� �븮��
	FSlotDragEventSignature OnSlotDragStarted;

	// �巡�� ��� �� ȣ��Ǵ� �븮��
	FSlotDragEventSignature OnSlotDragFinished;

	// �巡�� ��� �� ȣ��Ǵ� �븮��
	FSlotDragEventSignature OnSlotDragCancelled;


private :
	// ���� �巡�� �� �����Ǵ� ���� �������Ʈ UClass
	TSubclassOf<UUserWidget> BP_SlotDragImage;

protected :
	class UTexture2D* T_Null;

private :
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_SlotBackground;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Slot;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_ItemCount;


protected :
	UPROPERTY()
	ESlotType SlotType;

	// ������ ���԰� �Բ� ���Ǵ� �ڵ带 ��Ÿ���ϴ�.
	///  ex) ������ �ڵ�, ��ų �ڵ�
	FName InCode;

	// �巡�� �۾��� ����� �������� ��Ÿ���ϴ�.
	bool bAllowDragOperation;

public :
	UBaseSlot(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;

	// �� ������ ���콺 ��ħ�� ���۵Ǿ��� �� ȣ��Ǵ� �޼���
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;

	// �� ������ ���콺�� ��ħ�� ������ ��� ȣ��Ǵ� �޼���
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override final;
	
	// �ش� ������ ���콺 Ŭ�� �Է��� ���� ��� ȣ��˴ϴ�.
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	/// - FReply : �̺�Ʈ�� ��� ������� ó���Ǿ����� �˸��� ���� ����
	/// - InGeometry : ������ ��ġ, ũ�⸦ ��Ÿ���ϴ�.
	/// - InMouseEvent : ���콺 �Է� �̺�Ʈ�� ����
	
	// ���콺 Ŭ�� �Է��� ���� ��� ȣ��˴ϴ�.
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	
	// �� ������ ���� Ŭ���Ǿ��� ��� ȣ��˴ϴ�.
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
	
	// �ش� ������ �巡�� �̺�Ʈ�� �߻����� ��� ȣ��˴ϴ�.
	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override final;
	/// - OutOperation : ��¿� �Ű� �����̸�, ������ �巡�� �۾� ��ü�� ��ȯ�ؾ� �մϴ�.

	// �巡���� �巡�� ��� �� ȣ��Ǵ� �޼���
	virtual void NativeOnDragCancelled(
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override final;

	// �巡�װ� ������ ��� ȣ��Ǵ� �޼���
	virtual bool NativeOnDrop(
		const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override final;

public :
	// ������ �ʱ�ȭ�մϴ�.
	virtual void InitializeSlot(ESlotType slotType, FName inCode);
	
	// ���Կ� ǥ�õǴ� ���ڸ� �����մϴ�.
	/// - itemCount : ǥ�ý�ų ������ ������ �����մϴ�.
	/// - bVisible01Count : 0 �̳� 1 �� ��쿡�� ǥ���� �������� �����մϴ�.
	void SetSlotItemCount(int32 itemCount, bool bVisible01Count = false);

private :
	// ���� ����� �⺻ �������� ǥ���մϴ�.
	void ShowSlotNormalState();

	// ���� ����� ���콺 ��ħ ���·� ǥ���մϴ�.
	void ShowSlotHorverState();

	// ���� ����� ���� ���·� ǥ���մϴ�.
	void ShowSlotPressedState();

public :
	// ���� �̹��� ������ �⺻ / �巡�� �������� �����մϴ�.
	void SetSlotColorNormal();
	void SetSlotColorDragging();

protected :
	TTuple<UUserWidget*, class UImage*> CreateSlotDragWidget();
	/// - Ʃ��
	/// - ������ �̸��� ������ �ʰ�, ���� �ʵ带 ���� �� �մ� ����ü�Դϴ�.
	/// - ���� Ʃ���� �������α׷� ��ü���� ����� ������ ������ ���� �ƴ�,
	///   �Ͻ������� ����� ���� ������ ������ ������ �� ����մϴ�.
	/// - �𸮾󿡼� TTuple<Types...> �������� ������ �� ������, MakeTuple() �� ���� �ϳ��� �����͸� ������ �� �ֽ��ϴ�.
	/// - ������ Ʃ���� �ʵ忡�� Get<index>() �� ������ �� �ֽ��ϴ�.

public :
	FORCEINLINE class UImage* GetSlotBackgroundImage() const
	{ return Image_SlotBackground; }

	FORCEINLINE class UImage* GetSlotImage() const
	{ return Image_Slot; }

	FORCEINLINE ESlotType GetSlotType() const
	{ return SlotType; }
};
