#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DraggableWndTitle.generated.h"

DECLARE_MULTICAST_DELEGATE(FDraggingEventSignature)

UCLASS()
class ARPG_API UDraggableWndTitle final : 
	public UUserWidget
{
	GENERATED_BODY()

public :
	// �巡���� ���۵� �� ȣ��Ǵ� �븮��
	FDraggingEventSignature OnDraggingStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Title Text")
	FText TitleText;

private :
	// �巡�� ������ ��Ÿ���ϴ�.
	UPROPERTY()
	bool bIsDragging;

	// �巡�� ��ų ������ ��Ÿ���ϴ�.
	UPROPERTY()
	UUserWidget* TargetWidget;

private :
	// �巡���� üũ�� ��ư
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Draggable;
	
	// â �ݱ� ��ư
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Close;

	// â Ÿ��Ʋ �ؽ�Ʈ
	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Title;

	// ���� �Է� ��ġ�� ������ ����
	FVector2D PrevInputPosition;

	// ������ ���� ��ġ�� ������ ����
	FVector2D PrevWidgetPosition;

protected :
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private :
	// ���� �巡�׸� �����մϴ�.
	void DragWidget();

private :
	UFUNCTION()
	void OnWidgetDragStarted();

	UFUNCTION()
	void OnWidgetDragFinished();

public :
	void SetTitleText(FText newTitleText);

	FORCEINLINE void SetTargetWidget(UUserWidget* targetWidget)
	{ TargetWidget = targetWidget; }

	FORCEINLINE class UButton* GetCloseButton() const
	{ return Button_Close; }
	
};
