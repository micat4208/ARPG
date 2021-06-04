#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enum/InputModeType.h"
#include "ClosableWnd.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FWndEventSignature, class UClosableWnd*)

UCLASS()
class ARPG_API UClosableWnd : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY()
	class UWidgetControllerWidget* WidgetController;

	// â�� ���� �� ȣ��Ǵ� �븮��
	FWndEventSignature OnWndClosedEvent;

	// �θ� â�� ��Ÿ���ϴ�.
	/// - �θ� â�� nullptr �� �ƴ϶�� �θ� â�� ���� �� �ش� â�� ������ �˴ϴ�.
	UPROPERTY()
	UClosableWnd* ParentWnd;

	// �ڽ� â ��ü���� �����մϴ�.
	TArray<UClosableWnd*> ChildWnds;

	// ���� ó���� â���� ��Ÿ���ϴ�.
	UPROPERTY()
	bool bBeClose;



private :
	UPROPERTY(meta = (BindWidget))
	class UDraggableWndTitle * BP_DraggableWndTitle;

protected :
	UPROPERTY()
	class UButton * CloseButton;

	// â ũ�⸦ ��Ÿ���ϴ�.
	FVector2D WndSize;

protected :
	virtual void NativeConstruct() override;

public :
	// �ݱ� ��ư�� �����մϴ�.
	void SetCloseButton(class UButton* closeButtont);

	// �ڽ� â�� �����մϴ�.
	/// - closableWndClass : ������ â Ŭ������ �����մϴ�.
	/// - bUsePrevPosition : ���� â ��ġ ��� ���θ� �����մϴ�.
	/// - changeInputMode : �����ų �Է� ��带 �����մϴ�.
	/// - bShowMouseCursor : Ŀ�� ǥ�� ���θ� �����մϴ�.
	/// - return : ������ â�� ��ȯ�˴ϴ�.
	UClosableWnd* CreateChildClosableWnd(
		TSubclassOf<UClosableWnd> closableWndClass,
		bool bUsePrevPosition = false,
		EInputModeType changeInputMode = EInputModeType::IM_Default,
		bool bShowMouseCursor = true);

	// �� â�� �ݽ��ϴ�.
	UFUNCTION()
	virtual void CloseThisWnd();

	// ��� �ڽ� â�� �ݽ��ϴ�.
	void CloseAllChildWnd();

	// �θ� â���� ������ϴ�.
	void DetachFromParent();

public :
	// �� â�� CanvasPanelSlot �� ����ϴ�.
	class UCanvasPanelSlot* GetCanvasPanelSlot() const;

	// â�� ũ�⸦ ��ȯ�մϴ�.
	FORCEINLINE FVector2D GetWndSize() const
	{ return WndSize; }

	// â�� ������ �����մϴ�.
	void SetTitleText(FText newTitleText);
	FORCEINLINE void SetTitleText(FString newTitleText)
	{ SetTitleText(FText::FromString(newTitleText)); }
	

	
};
