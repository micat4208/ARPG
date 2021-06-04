#include "WidgetControllerWidget.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridSlot.h"

#include "Widget/ClosableWnd/MessageBoxWnd/MessageBoxWnd.h"
#include "Widget/ClosableWnd/ClosableWnd.h"


UWidgetControllerWidget::UWidgetControllerWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMessageBoxWnd> BP_MESSAGE_BOX_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/MessageBoxWnd/BP_MessageBoxWnd.BP_MessageBoxWnd_C'"));
	if (BP_MESSAGE_BOX_WND.Succeeded()) BP_MessageBoxWnd = BP_MESSAGE_BOX_WND.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> BP_MESSAGE_BOX_BACKGROUND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/MessageBoxWnd/BP_MessageBoxBackground.BP_MessageBoxBackground_C'"));
	if (BP_MESSAGE_BOX_BACKGROUND.Succeeded()) BP_MessageBoxBackground = BP_MESSAGE_BOX_BACKGROUND.Class;
}

void UWidgetControllerWidget::InitializeWidgetControllerWidget(
	class ABasePlayerController* basePlayerController)
{
	PlayerController = basePlayerController;
}

void UWidgetControllerWidget::ResetInputMode(bool bForceChange)
{
	// ���� â�� ������ �������� �ʴ´ٸ� �Է� ��带 �����մϴ�.
	if (bForceChange ||
		((AllocatedWnds.Num() == 0) && (AllocatedWidgets.Num() == 0)))
	{
		// �Է� ��带 ����
		PlayerController->ChangeInputModeToDefault();

		// Ŀ�� ǥ��
		PlayerController->bShowMouseCursor = PlayerController->GetDefaultCursorVisibility();
	}
}

void UWidgetControllerWidget::SaveWndPosition(const UClosableWnd* closableWnd)
{
	FString wndClsName = closableWnd->GetClass()->GetName();
	FVector2D wndPosition = closableWnd->GetCanvasPanelSlot()->GetPosition();

	if (PrevClosedWndPositions.Contains(wndClsName))
		PrevClosedWndPositions[wndClsName] = wndPosition;

	else PrevClosedWndPositions.Add(wndClsName, wndPosition);
}

UMessageBoxWnd* UWidgetControllerWidget::CreateMessageBox(FText titleText, FText msg, bool bUseBackground, uint8 buttons)
{
	UUserWidget* msgBoxBackground = nullptr;

	if (bUseBackground)
	{
		msgBoxBackground = CreateWidget<UUserWidget>(this, BP_MessageBoxBackground);
		CanvasPanel_WndParent->AddChild(msgBoxBackground);

		UCanvasPanelSlot* messageBoxCanvasPanelSlot = Cast<UCanvasPanelSlot>(msgBoxBackground->Slot);
		messageBoxCanvasPanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
		messageBoxCanvasPanelSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
	}

	UMessageBoxWnd* msgBox = Cast<UMessageBoxWnd>(
		CreateWnd(BP_MessageBoxWnd, false, EInputModeType::IM_UIOnly, true));

	msgBox->MsgBoxBackground = msgBoxBackground;

	msgBox->InitializeMessageBox(titleText, msg, buttons);

	return msgBox;
}

UClosableWnd* UWidgetControllerWidget::CreateWnd(
	TSubclassOf<class UClosableWnd> wndClass, 
	bool bUsePrevPosition, 
	EInputModeType changeInputMode, 
	bool bShowMouseCursor, 
	float alignmentX, float alignmentY, 
	float anchorMinX, float anchorMinY, 
	float anchorMaxX, float anchorMaxY)
{
	// ���ο� â ������ �����մϴ�.
	UClosableWnd* newClosableWnd = CreateWidget<UClosableWnd>(this, wndClass);
	newClosableWnd->WidgetController = this;

	// �Ҵ�� â �迭�� �߰��մϴ�.
	AllocatedWnds.Add(newClosableWnd);

	// CanvasPanel_WndParent �� �ڽ� �������� �߰��մϴ�.
	CanvasPanel_WndParent->AddChild(newClosableWnd);

	// Anchor / Alignment ����
	auto wndCanvasPanelSlot = newClosableWnd->GetCanvasPanelSlot();
	wndCanvasPanelSlot->SetAnchors(FAnchors(anchorMinX, anchorMinY, anchorMaxX, anchorMaxY));
	wndCanvasPanelSlot->SetAlignment(FVector2D(alignmentX, alignmentY));

	// â ũ�⸦ �����մϴ�.
	wndCanvasPanelSlot->SetSize(newClosableWnd->GetWndSize());

	// ���� â ��ġ�� ����Ѵٸ�
	if (bUsePrevPosition)
	{
		// ���� â ��ġ�� ��ϵǾ� �ִ��� Ȯ���մϴ�.
		//if (PrevClosedWndPositions.Contains(newClosableWnd->GetClass()->GetName()))
		if (PrevClosedWndPositions.Contains(wndClass->GetName()))
		{
			// â ��ġ�� ���� ��ġ�� �����մϴ�.
			wndCanvasPanelSlot->SetPosition(PrevClosedWndPositions[wndClass->GetName()]);
		}
	}

	// �Է� ���� Ŀ�� ǥ�� ���θ� �����մϴ�.
	if (changeInputMode != EInputModeType::IM_Default)
		PlayerController->SetInputModeFromNewInputModeType(changeInputMode);
	PlayerController->bShowMouseCursor = bShowMouseCursor;

	// ������ â ��ü�� ��ȯ�մϴ�.
	return newClosableWnd;
}

void UWidgetControllerWidget::CloseWnd(bool bAllClose, UClosableWnd* closableWndInstanceToClose)
{
	// ���� ���� â�� �������� �ʴ� ��� �������� �ʽ��ϴ�.
	if (AllocatedWnds.Num() == 0) return;

	// ��� â�� �ݵ��� �Ͽ��ٸ�
	if (bAllClose)
	{
		for (auto wnd : AllocatedWnds)
		{
			// ���� ó���� â�̶�� �������� �ʽ��ϴ�.
			if (wnd->bBeClose) continue;
			wnd->bBeClose = true;

			// â ���� �̺�Ʈ�� �߻���ŵ�ϴ�.
			if (wnd->OnWndClosedEvent.IsBound())
				wnd->OnWndClosedEvent.Broadcast(wnd);

			// ��� �ڽ� â�� �ݽ��ϴ�.
			wnd->CloseAllChildWnd();

			// â ��ġ�� �����մϴ�.
			SaveWndPosition(wnd);

			// �θ� â���� ������ϴ�.
			wnd->DetachFromParent();

			// CanvasPanel_WndParent ������ �ڽĿ��� �����մϴ�.
			CanvasPanel_WndParent->RemoveChild(wnd);
		}

		// �״�� â �迭�� ���ϴ�.
		AllocatedWnds.Empty();
	}
	// Ư���� â�� �ݵ��� �Ͽ��ٸ�
	else
	{
		// ���� â�� �������� �ʾҴٸ� ���������� ���� â�� ���� â���� �����մϴ�.
		closableWndInstanceToClose = (closableWndInstanceToClose != nullptr) ?
			closableWndInstanceToClose :
			AllocatedWnds[AllocatedWnds.Num() - 1];

		// �� ���� â�� �ƴ϶��
		if (!closableWndInstanceToClose->bBeClose)
		{
			// ���� â���� �����մϴ�.
			closableWndInstanceToClose->bBeClose = true;

			// â ���� �̺�Ʈ�� �߻���ŵ�ϴ�.
			if (closableWndInstanceToClose->OnWndClosedEvent.IsBound())
				closableWndInstanceToClose->OnWndClosedEvent.Broadcast(closableWndInstanceToClose);

			// ��� �ڽ� â�� �����մϴ�.
			closableWndInstanceToClose->CloseAllChildWnd();

			// �Ҵ�� â �迭���� �����մϴ�.
			AllocatedWnds.Remove(closableWndInstanceToClose);

			// â ��ġ�� �����մϴ�.
			SaveWndPosition(closableWndInstanceToClose);

			// �θ� â���� ������ϴ�.
			closableWndInstanceToClose->DetachFromParent();

			// CanvasPanel_WndParent ������ �ڽĿ��� �����մϴ�.
			CanvasPanel_WndParent->RemoveChild(closableWndInstanceToClose);
		}

	}

	// �Է� ��� �ʱ�ȭ
	ResetInputMode();
}

void UWidgetControllerWidget::AddChildWidget(UUserWidget* childWidgetInstance,
	EInputModeType changeInputMode, bool bShowMouseCursor,
	float width, float height)
{
	if (childWidgetInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, 
			TEXT("WidgetControllerWidget.cpp :: %d LINE :: childWidgetInstance is nullptr"), __LINE__);
		return;
	}

	CanvasPanel_WidgetParent->AddChild(childWidgetInstance);
	/// - Widget->AddChild(childWidget) : childWidget �� Widget �� ������ �߰��մϴ�.

	Cast<UCanvasPanelSlot>(childWidgetInstance->Slot)->SetSize(FVector2D(width, height));

	// �Է� ���� Ŀ�� ���ü��� �����մϴ�.
	PlayerController->SetInputModeFromNewInputModeType(changeInputMode);
	PlayerController->bShowMouseCursor = bShowMouseCursor;

	AllocatedWidgets.Add(childWidgetInstance);

}

void UWidgetControllerWidget::CloseChildWidget(UUserWidget* childWidgetInstance)
{
	if (childWidgetInstance == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("WidgetControllerWidget.cpp :: %d LINE :: childWidgetInstance is nullptr"), __LINE__);
		return;
	}

	// �����Ϸ��� ������ AllocatedWidgets ���� �����մϴ�.
	AllocatedWidgets.Remove(childWidgetInstance);

	// �����Ϸ��� ������ ȭ�鿡�� ������ϴ�.
	CanvasPanel_WidgetParent->RemoveChild(childWidgetInstance);
	/// - �θ� �������� �ڽ� ������ �����մϴ�.

	//childWidgetInstance->RemoveFromParent();
	/// - �ڽ� �������� �θ�� ������ �����ϴ�.

	// �Է� ��带 �⺻ ������ �����մϴ�.
	ResetInputMode();
}

void UWidgetControllerWidget::SetHighestPriorityWnd(UClosableWnd* closableWndInstance)
{
	// �켱������ �����ų ������ ������ ����ϴ�.
	UCanvasPanelSlot* wndSlot = closableWndInstance->GetCanvasPanelSlot();

	// �ֻ������ �����Ϸ��� ������ �ֻ�ܿ� ��ġ�Ǿ� ���� �ʴٸ�
	if (CanvasPanel_WndParent->GetChildIndex(closableWndInstance) !=
		/// - GetChildIndex(widget) : widget �� �������� ������ ��ȯ�մϴ�.
		CanvasPanel_WndParent->GetSlots().Num() - 1)
		/// - GetSlots() : �߰��� �������� ���� ������ �����ϴ� �迭�� ��ȯ�մϴ�.
	{
		// ��ġ, Anchor, Alignment �� �����մϴ�.
		FVector2D prevPosition = wndSlot->GetPosition();
		FAnchors prevAnchors = wndSlot->GetAnchors();
		FVector2D prevAlignment = wndSlot->GetAlignment();

		// ������ �����մϴ�.
		CanvasPanel_WndParent->AddChild(closableWndInstance);

		// Anchor, Alignment, ��ġ, ũ�⸦ �缳���մϴ�.
		wndSlot = closableWndInstance->GetCanvasPanelSlot();
		wndSlot->SetAnchors(prevAnchors);
		wndSlot->SetAlignment(prevAlignment);
		wndSlot->SetPosition(prevPosition);
		wndSlot->SetSize(closableWndInstance->GetWndSize());
	}
}

void UWidgetControllerWidget::SortGridPanelElem(
	UUserWidget* gridPanelElem, 
	int maxColumnCount, 
	int& refCurrentColumCount)
{
	UGridSlot* gridSlot = Cast<UGridSlot>(gridPanelElem->Slot);

	if (!IsValid(gridSlot)) return;

	// �׸��� ��� ���� �����մϴ�.
	gridSlot->SetColumn(refCurrentColumCount % maxColumnCount);
	gridSlot->SetRow(refCurrentColumCount / maxColumnCount);
	++refCurrentColumCount;
}
