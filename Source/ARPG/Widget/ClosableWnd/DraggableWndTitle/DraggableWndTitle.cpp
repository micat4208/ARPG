#include "DraggableWndTitle.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UDraggableWndTitle::NativePreConstruct()
{
	Super::NativePreConstruct();
	Text_Title->SetText(TitleText);
}

void UDraggableWndTitle::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Draggable->OnPressed.AddDynamic(this, &UDraggableWndTitle::OnWidgetDragStarted);
	Button_Draggable->OnReleased.AddDynamic(this, &UDraggableWndTitle::OnWidgetDragFinished);
}

void UDraggableWndTitle::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DragWidget();

}

void UDraggableWndTitle::DragWidget()
{
	// �巡�� ���� �ƴ϶�� �������� �ʽ��ϴ�.
	if (!bIsDragging) return;

	// �̵� ��ų ������ �������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (!IsValid(TargetWidget)) return;

	// ���콺 ��ġ�� ����ϴ�.
	auto mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// �巡�� ��ų ���� ������ ����ϴ�.
	auto targetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);

	// ������ ��ġ�� �����մϴ�.
	targetSlot->SetPosition(
		PrevWidgetPosition + (mousePosition - PrevInputPosition));

	PrevInputPosition = mousePosition;
	PrevWidgetPosition = targetSlot->GetPosition();
}

void UDraggableWndTitle::OnWidgetDragStarted()
{
	// �̵���ų ������ �������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (!IsValid(TargetWidget)) return;

	// �Էµ� ���콺 ��ġ�� �����մϴ�.
	PrevInputPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	/// - UWidgetLayoutLibrary : ���� ���̾ƿ��� ���õ� ��ƿ�� ���� �޼������ �����ϴ� Ŭ����
	/// - GetMousePositionOnViewport : ����Ʈ ���� ���콺 ��ġ�� ��ȯ�մϴ�.

	// ������ ���� ��ġ�� �����մϴ�.
	PrevWidgetPosition = Cast<UCanvasPanelSlot>(TargetWidget->Slot)->GetPosition();

	if (OnDraggingStarted.IsBound())
		OnDraggingStarted.Broadcast();

	bIsDragging = true;
}

void UDraggableWndTitle::OnWidgetDragFinished()
{
	// �̵���ų ������ �������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (!IsValid(TargetWidget)) return;

	bIsDragging = false;
}

void UDraggableWndTitle::SetTitleText(FText newTitleText)
{ Text_Title->SetText(newTitleText); }
