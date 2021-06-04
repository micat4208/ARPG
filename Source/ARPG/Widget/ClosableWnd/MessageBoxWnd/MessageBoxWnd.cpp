#include "MessageBoxWnd.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/DraggableWndTitle/DraggableWndTitle.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMessageBoxWnd::SetButtonVisibility(
	UButton*					button, 
	EMessageBoxButton::Type		buttonType, 
	uint8						visibility)
{
	ESlateVisibility hide		= ESlateVisibility::Collapsed;
	ESlateVisibility visible	= ESlateVisibility::Visible;

	button->SetVisibility(((visibility & (int8)buttonType) == 0) ? hide : visible);
}

void UMessageBoxWnd::InitializeMessageBox(FText titleText, FText msg, uint8 buttons)
{
	// â ���� ����
	SetTitleText(titleText);

	// �޽��� ����
	Text_Message->SetText(msg);

	// ��ư ǥ��
	SetButtonVisibility(Button_Ok, EMessageBoxButton::MB_Ok, buttons);
	SetButtonVisibility(Button_Cancel, EMessageBoxButton::MB_Cancel, buttons);
}

void UMessageBoxWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// â ũ�� ����
	WndSize = FVector2D(500.0f, 300.0f);

	Button_Ok->OnClicked.AddDynamic(this, &UMessageBoxWnd::OnOkButtonClicked);
	Button_Cancel->OnClicked.AddDynamic(this, &UMessageBoxWnd::OnCancelButtonClicked);
}

void UMessageBoxWnd::CloseThisWnd()
{
	if (IsValid(MsgBoxBackground))
	{
		MsgBoxBackground->RemoveFromParent();
		MsgBoxBackground = nullptr;
	}

	Super::CloseThisWnd();
}

void UMessageBoxWnd::OnOkButtonClicked()
{
	OnOkButtonClickedEvent.ExecuteIfBound(WidgetController, this);
}

void UMessageBoxWnd::OnCancelButtonClicked()
{
	OnCancelButtonClickedEvent.ExecuteIfBound(WidgetController, this);
}
