#include "NpcDialog.h"

#include "Actor/Controller/PlayerController/BasePlayerController.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/WidgetControllerWidget/WidgetControllerWidget.h"
#include "Widget/ClosableWnd/NpcShopWnd/NpcShopWnd.h"
#include "Widget/ClosableWnd/NpcQuestListWnd/NpcQuestListWnd.h"

#include "Struct/QuestInfo/QuestInfo.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

UNpcDialog::UNpcDialog(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UNpcShopWnd> BP_NPC_SHOP_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/NpcShopWnd/BP_NpcShopWnd.BP_NpcShopWnd_C'"));
	if (BP_NPC_SHOP_WND.Succeeded()) BP_NpcShopWnd = BP_NPC_SHOP_WND.Class;

	static ConstructorHelpers::FClassFinder<UNpcQuestListWnd> BP_NPC_QUEST_LIST_WND(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/NpcQuestListWnd/BP_NpcQuestListWnd.BP_NpcQuestListWnd_C'"));
	if (BP_NPC_QUEST_LIST_WND.Succeeded()) BP_NpcQuestListWnd = BP_NPC_QUEST_LIST_WND.Class;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHOP_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_ShopInfo.DT_ShopInfo'"));
	if (DT_SHOP_INFO.Succeeded()) DT_ShopInfo = DT_SHOP_INFO.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_QUEST_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_QuestInfo.DT_QuestInfo'"));
	if (DT_QUEST_INFO.Succeeded()) DT_QuestInfo = DT_QUEST_INFO.Object;

	NpcShopWnd = nullptr;
	NpcQuestListWnd = nullptr;
}

void UNpcDialog::NativeConstruct()
{
	Super::NativeConstruct();

	// �� ������ ��Ŀ���� �����ϵ��� �մϴ�.
	bIsFocusable = true;
	SetKeyboardFocus();
	// Ű �Է��� ��Ŀ�� �� ���¿��� �̷������ �մϴ�.

	Button_Exit->OnClicked.AddDynamic(this, &UNpcDialog::OnExitButtonClicked);
	Button_Next->OnClicked.AddDynamic(this, &UNpcDialog::OnNextDialogButtonClicked);
	Button_Accept->OnClicked.AddDynamic(this, &UNpcDialog::OnAcceptButtonClicked);
	Button_Rejection->OnClicked.AddDynamic(this, &UNpcDialog::InitializeDialog);
	Button_Quest->OnClicked.AddDynamic(this, &UNpcDialog::OnQuestButtonClicked);
	Button_Shop->OnClicked.AddDynamic(this, &UNpcDialog::CreateShopWnd);
	Button_GoToFirst->OnClicked.AddDynamic(this, &UNpcDialog::InitializeDialog);
}

FReply UNpcDialog::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::R)
	{
		// ���� ��ȭ ������ ǥ���մϴ�.
		OnNextDialogButtonClicked();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UNpcDialog::InitializeNpcDialog(FNpcInfo* npcInfo)
{
	NpcInfo = npcInfo;

	if (NpcInfo->IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("NpcDialog.cpp :: %d LINE :: NpcInfo is Empty!"), __LINE__);
		return;
	}

	// Npc �̸� ǥ��
	Text_NpcName->SetText(NpcInfo->NpcName);

	// ���̾�α� �ʱ�ȭ
	InitializeDialog();

	// �����̳� ����Ʈ�� ������� �ʴ� Npc ��� ��ư�� ����ϴ�.
	SetMenuButtonVisibility(NpcInfo->IsShopUsing(), NpcInfo->IsQuestUsing());
}

void UNpcDialog::InitializeQuestInfos()
{
	QuestInProgress.Empty();
	OrderableQuests.Empty();

	for (auto code : NpcInfo->QuestCodes)
	{
		FString contextString;
		FQuestInfo* questInfo = DT_QuestInfo->FindRow<FQuestInfo>(code, contextString);

		// �������� ����Ʈ�� ã�� QuestInProgress �� �����մϴ�.
		if (FQuestInfo::IsProgress(GetManager(UPlayerManager), code))
			QuestInProgress.Add(code, questInfo);
		
		// ���� ������ ����Ʈ�� ã�� OrderbleQuests �� �����մϴ�.
		if (questInfo->IsOrderable(GetManager(UPlayerManager), code))
			OrderableQuests.Add(code, questInfo);
	}
}

void UNpcDialog::SetButtonVisibility(class UButton* button, bool bVisible)
{
	button->SetVisibility(
		bVisible ?
		ESlateVisibility::Visible :
		ESlateVisibility::Collapsed);
}

void UNpcDialog::SetMenuButtonVisibility(bool bShopButtonVisible, bool bQuestButtonVisible)
{
	SetButtonVisibility(Button_Shop, bShopButtonVisible);
	SetButtonVisibility(Button_Quest, bQuestButtonVisible);
}

void UNpcDialog::ShowDialog(int32 newDialogIndex)
{
	// ��� ������ ��ȭ �ؽ�Ʈ�� �������� �ʴ´ٸ�
	if (DialogInfos.DialogText.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NpcDialog.cpp :: %d LINE :: Usable Dialog Count Is Zero!"), __LINE__);
		return;
	}

	// ǥ���Ϸ��� ��ȭ �ε����� �迭 ������ �ʰ��Ѵٸ�
	else if (DialogInfos.DialogText.Num() <= newDialogIndex)
	{
		UE_LOG(LogTemp, Error,
			TEXT("NpcDialog.cpp :: %d LINE :: [Warning] Out Of Range! newDialogIndex is Changed (%d -> %d)"),
			__LINE__,
			newDialogIndex,
			DialogInfos.DialogText.Num() - 1);

		// ǥ���Ϸ��� ��ȭ �ε����� ������ ��ȭ �ε����� �����մϴ�.
		newDialogIndex = (DialogInfos.DialogText.Num() - 1);
	}

	// �ؽ�Ʈ ����
	Text_Dialog->SetText(DialogInfos.DialogText[newDialogIndex]);

	// ������ ��ȭ���� Ȯ���մϴ�.
	bIsLastDialog = (DialogInfos.DialogText.Num() - 1) == newDialogIndex;

	// ������ ��ȭ���
	if (bIsLastDialog)
	{
		// ���� ��ư �����
		SetButtonVisibility(Button_Next, false);

		// ����Ʈ�� ��ȭ���
		if (bIsQuestDialog)
		{
			// ���� / ���� ��ư ǥ��
			SetButtonVisibility(Button_Rejection, true);
			SetButtonVisibility(Button_Accept, true);
		}
	}
	else
	{
		// ���� ��ȭ ��ư ǥ��
		SetButtonVisibility(Button_Next, true);
	}
}

void UNpcDialog::InitializeDialog()
{
	bIsLastDialog = false;
	bIsQuestDialog = false;
	AcceptableQuestCode = FName();

	DialogInfos = NpcInfo->DefaultDialogInfo;
	CurrentDialogIndex = 0;

	// ��ȭ ���� ǥ��
	ShowDialog(CurrentDialogIndex);

	// ����Ʈ ���� �ʱ�ȭ
	InitializeQuestInfos();

	// ����Ʈ ���� / ���� ��ư �����
	SetButtonVisibility(Button_Rejection, false);
	SetButtonVisibility(Button_Accept, false);

	// ����Ʈ ���� ��ư �̺�Ʈ ����
	OnAcceptButtonClickedOneshot.Clear();
}

void UNpcDialog::ChangeDialogInfo(FNpcDialogInfo newDialogInfo, bool bIsQuestDlg)
{
	// ��ȭ ���� ����
	DialogInfos = newDialogInfo;

	bIsQuestDialog = bIsQuestDlg;

	SetKeyboardFocus();

	// ��ȭ �ʱ�ȭ
	CurrentDialogIndex = 0;
	ShowDialog(CurrentDialogIndex);

	AcceptableQuestCode = FName();
}

void UNpcDialog::CreateShopWnd()
{
	if (IsValid(NpcShopWnd)) return;

	InitializeDialog();

	FString contextString;
	FShopInfo* shopInfo = DT_ShopInfo->FindRow<FShopInfo>(NpcInfo->ShopCode, contextString);

	NpcShopWnd = Cast<UNpcShopWnd>(
		GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget()->CreateWnd(BP_NpcShopWnd, true));

	// �κ��丮 â�� ���ϴ�.
	NpcShopWnd->FloatingInventoryWnd();
	NpcShopWnd->InitializeNpcShop(shopInfo);

	NpcShopWnd->OnWndClosedEvent.AddLambda(
		[this](UClosableWnd* closableWnd)
		{
			NpcShopWnd = nullptr;
		});
}

void UNpcDialog::OnQuestButtonClicked()
{
	if (IsValid(NpcQuestListWnd)) return;

	NpcQuestListWnd = Cast<UNpcQuestListWnd>(
		GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget()->CreateWnd(BP_NpcQuestListWnd));

	NpcQuestListWnd->NpcDialog = this;

	// ����Ʈ ���� ����
	NpcQuestListWnd->UpdateQuestInfo(OrderableQuests, QuestInProgress);

	// ����Ʈ ����Ʈ ����
	NpcQuestListWnd->UpdateQuestList();


	NpcQuestListWnd->OnWndClosedEvent.AddLambda(
		[this](UClosableWnd* closableWnd)
		{
			NpcQuestListWnd = nullptr;
		});
}

void UNpcDialog::OnExitButtonClicked()
{
	auto widgetController = GetManager(UPlayerManager)->GetPlayerController()->GetWidgetControllerWidget();

	if (OnDlgClosed.IsBound())
		OnDlgClosed.Broadcast();

	widgetController->CloseChildWidget(this);

	// ���� â �ݱ�
	if (IsValid(NpcShopWnd))
		widgetController->CloseWnd(false, NpcShopWnd);
}

void UNpcDialog::OnNextDialogButtonClicked()
{
	// ���� ǥ������ ��ȭ�� ������ ��ȭ��� ���� ��ȭ�� ǥ������ �ʽ��ϴ�.
	if (DialogInfos.DialogText.Num() - 1 <= CurrentDialogIndex)
		return;

	ShowDialog(++CurrentDialogIndex);

}

void UNpcDialog::OnAcceptButtonClicked()
{
	OnAcceptButtonClickedOneshot.Broadcast();
	OnAcceptButtonClickedOneshot.Clear();

	FString contextString;
	FQuestInfo* questInfo = DT_QuestInfo->FindRow<FQuestInfo>(AcceptableQuestCode, contextString);
	
	GetManager(UPlayerManager)->GetPlayerInfo()->AddProgressQuest(AcceptableQuestCode, *questInfo);

	LOG(TEXT("Quest Start! [%s]"), * AcceptableQuestCode.ToString());

	InitializeDialog();
}