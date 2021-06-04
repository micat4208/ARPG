#pragma once

#include "CoreMinimal.h"
#include "Widget/ClosableWnd/ClosableWnd.h"
#include "Enum/MessageBoxButton.h"
#include "MessageBoxWnd.generated.h"

DECLARE_DELEGATE_TwoParams(FOnButtonEventSignature, class UWidgetControllerWidget *, class UMessageBoxWnd*)

UCLASS()
class ARPG_API UMessageBoxWnd : public UClosableWnd
{
	GENERATED_BODY()

public :
	// Ok ��ư�� Ŭ���Ǿ��� �� ȣ��Ǵ� �븮��
	FOnButtonEventSignature OnOkButtonClickedEvent;

	// Cancel ��ư�� Ŭ���Ǿ��� �� ȣ��Ǵ� �븮��
	FOnButtonEventSignature OnCancelButtonClickedEvent;

public :
	// �Բ� ���Ǵ� ��� ������ ��Ÿ���ϴ�.
	UPROPERTY()
	class UUserWidget * MsgBoxBackground;

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Message;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Ok;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Cancel;

private :
	// ��ư�� ���ü��� �����մϴ�.
	/// - button : ������ ��ư�� �����մϴ�.
	/// - buttonType : ��ư�� Ÿ���� �����մϴ�.
	/// - visibility : ǥ�� ���θ� ��Ÿ���� �����͸� �����մϴ�.
	void SetButtonVisibility(class UButton* button, EMessageBoxButton::Type buttonType, uint8 visibility);

public :
	// �޽��� �ڽ��� �ʱ�ȭ�մϴ�.
	/// - titleText : �޽��� �ڽ��� Ÿ��Ʋ ���ڿ��� �����մϴ�.
	/// - msg : ǥ���� �޽��� ���ڿ��� �����մϴ�.
	/// - buttons : ǥ���� ��ư�� ��Ʈ������ OR �����ڿ� �Բ� �����մϴ�.
	void InitializeMessageBox(FText titleText, FText msg, uint8 buttons);

protected :
	virtual void NativeConstruct() override;

public :
	virtual void CloseThisWnd() override;

private :
	UFUNCTION()
	void OnOkButtonClicked();

	UFUNCTION()
	void OnCancelButtonClicked();


	
};
