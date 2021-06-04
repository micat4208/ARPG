#include "ClosableWnd.h"

#include "Components/CanvasPanelSlot.h"

#include "Widget/ClosableWnd/DraggableWndTitle/DraggableWndTitle.h"
#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"

#include "Components/Button.h"

void UClosableWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// â ũ�⸦ �����մϴ�.
	WndSize = FVector2D(500.0f, 600.0f);

	BP_DraggableWndTitle->SetTargetWidget(this);

	// �巡���� ���۵� �� �� â�� �ֻ������ ��ġ�ϵ��� �մϴ�.
	BP_DraggableWndTitle->OnDraggingStarted.AddLambda(
		[this]() { WidgetController->SetHighestPriorityWnd(this); });

	SetCloseButton(BP_DraggableWndTitle->GetCloseButton());

}

void UClosableWnd::SetCloseButton(UButton* closeButtont)
{
	(CloseButton = closeButtont)->OnClicked.AddDynamic(this, &UClosableWnd::CloseThisWnd);
}

UClosableWnd* UClosableWnd::CreateChildClosableWnd(TSubclassOf<UClosableWnd> closableWndClass, 
	bool bUsePrevPosition, EInputModeType changeInputMode, bool bShowMouseCursor)
{
	// �ڽ� â�� �����մϴ�.
	auto childWnd = WidgetController->CreateWnd(
		closableWndClass, bUsePrevPosition,
		changeInputMode, bShowMouseCursor);

	// ������ �ڽ� â�� �θ� â�� �ڽ����� �����մϴ�.
	childWnd->ParentWnd = this;

	// �迭�� �߰��մϴ�.
	ChildWnds.Add(childWnd);

	// ������ �ڽ� â�� ��ȯ�մϴ�.
	return childWnd;
}

void UClosableWnd::CloseThisWnd()
{
	if (bBeClose) return;

	WidgetController->CloseWnd(false, this);
}

void UClosableWnd::CloseAllChildWnd()
{
	// �ڽ� â�� �����Ѵٸ�
	if (ChildWnds.Num() != 0)
	{
		// ��� �ڽ� â�� �����մϴ�.
		for (int32 i = 0; i < ChildWnds.Num(); ++i)
		{ 
			auto childWnd = ChildWnds[i];

			if (IsValid(childWnd))
			{
				// �� ���� â�̶�� �������� �ʽ��ϴ�.
				if (childWnd->bBeClose) continue;

				WidgetController->CloseWnd(
					/*bAllClose = */ false,
					/*closableWndInstanceToClose = */ childWnd);
			}
		}

		ChildWnds.Empty();
	}
}

void UClosableWnd::DetachFromParent()
{
	// �θ� â�� �����Ѵٸ�
	if (IsValid(ParentWnd))
	{
		ParentWnd->ChildWnds.Remove(this);
	}
}

UCanvasPanelSlot* UClosableWnd::GetCanvasPanelSlot() const
{
	return Cast<UCanvasPanelSlot>(Slot);
}

void UClosableWnd::SetTitleText(FText newTitleText)
{
	BP_DraggableWndTitle->SetTitleText(newTitleText);
}
