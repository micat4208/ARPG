#pragma once

#include "ARPG.h"
#include "Blueprint/UserWidget.h"

#include "Struct/NpcInfo/NpcInfo.h"
#include "Struct/NpcDialogInfo/NpcDialogInfo.h"

#include "NpcDialog.generated.h"

DECLARE_MULTICAST_DELEGATE(FNpcDialogEventSignature)

DECLARE_EVENT(UNpcDialog, FQuestButtonEvent)

UCLASS()
class ARPG_API UNpcDialog final : 
	public UUserWidget
{
	GENERATED_BODY()

private :
	TSubclassOf<class UNpcShopWnd> BP_NpcShopWnd;
	TSubclassOf<class UNpcQuestListWnd> BP_NpcQuestListWnd;

	class UDataTable* DT_ShopInfo;
	class UDataTable* DT_QuestInfo;

public :
	// Npc Dialog Widget �� ���� ��� ȣ��� �븮��
	FNpcDialogEventSignature OnDlgClosed;

	// ����Ʈ ���� ��ư�� ������ �� �߻��ϴ� �̺�Ʈ
	FQuestButtonEvent OnAcceptButtonClickedOneshot;

private :
	// Npc ���� â ��ü�� ��Ÿ���ϴ�.
	class UNpcShopWnd* NpcShopWnd;

	// Npc ����Ʈ â ��ü�� ��Ÿ���ϴ�.
	class UNpcQuestListWnd * NpcQuestListWnd;

	FNpcInfo* NpcInfo;

	// �������� ����Ʈ ���
	TMap<FName, struct FQuestInfo*> QuestInProgress;


	// ���� ������ ����Ʈ ���
	TMap<FName, struct FQuestInfo*> OrderableQuests;

	// ǥ�õǴ� ��ȭ ������ ��Ÿ���ϴ�.
	FNpcDialogInfo DialogInfos;

	// ����Ʈ�� ��ȭ���� ��Ÿ���ϴ�.
	bool bIsQuestDialog;

	// ���� �� ��° ��ȭ�� ǥ���ϴ� �� ��Ÿ���ϴ�.
	UPROPERTY()
	int32 CurrentDialogIndex;

	// ������ ��ȭ���� ��Ÿ���ϴ�.
	bool bIsLastDialog;

	// ���� ������ ����Ʈ �ڵ带 ��Ÿ���ϴ�.
	FName AcceptableQuestCode;


private :
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Exit;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Next;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Accept;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Rejection;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_GoToFirst;
	
	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Shop;

	UPROPERTY(meta = (BindWidget))
	class UButton * Button_Quest;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_NpcName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock * Text_Dialog;

public :
	UNpcDialog(const FObjectInitializer& ObjectInitializer);

protected :
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public :
	// ���̾�α� ��ü �ʱ�ȭ
	void InitializeNpcDialog(FNpcInfo* npcInfo);

	// ��ȭ ������ �����մϴ�.
	void ChangeDialogInfo(FNpcDialogInfo newDialogInfo, bool bIsQuestDlg = false);

private :
	// ����Ʈ ���� �ʱ�ȭ
	void InitializeQuestInfos();

	// ��ư�� ���ü��� �����մϴ�.
	void SetButtonVisibility(class UButton* button, bool bVisible);

	// ������ �ʴ� �޴��� ���ü��� �����մϴ�.
	void SetMenuButtonVisibility(bool bShopButtonVisible, bool bQuestButtonVisible);

	// ������ ������ ��ȭ�� ǥ���մϴ�.
	void ShowDialog(int32 newDialogIndex);


#pragma region ButtonEvent

private :
	// ���̾�α׸� �ʱ�ȭ�մϴ�.
	UFUNCTION()
	void InitializeDialog();

	UFUNCTION()
	void CreateShopWnd();

	UFUNCTION()
	void OnQuestButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UFUNCTION()
	void OnNextDialogButtonClicked();

	UFUNCTION()
	void OnAcceptButtonClicked();

#pragma endregion

public :
	// ���� ������ ����Ʈ �ڵ带 �����մϴ�.
	FORCEINLINE void SetAcceptableQuestCode(FName acceptableQuestCode)
	{ AcceptableQuestCode = acceptableQuestCode; }

	
};
