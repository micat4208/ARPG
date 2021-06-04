#include "NpcQuestListWnd.h"

#include "Widget/ClosableWnd/NpcQuestListWnd/NpcQuestListElem/NpcQuestListElem.h"
#include "Widget/NpcDialog/NpcDialog.h"

#include "Struct/QuestInfo/QuestInfo.h"

#include "Components/ScrollBox.h"

UNpcQuestListWnd::UNpcQuestListWnd(const FObjectInitializer& objInitializer)
{
	static ConstructorHelpers::FClassFinder<UNpcQuestListElem> BP_NPC_QUEST_LIST_ELEM(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/ClosableWnd/NpcQuestListWnd/BP_NpcQuestListElem.BP_NpcQuestListElem_C'"));
	if (BP_NPC_QUEST_LIST_ELEM.Succeeded()) BP_NpcQuestListElem = BP_NPC_QUEST_LIST_ELEM.Class;
}

void UNpcQuestListWnd::NativeConstruct()
{
	Super::NativeConstruct();

	WndSize = FVector2D(500.0f, 600.0f);
}

void UNpcQuestListWnd::UpdateQuestList()
{
	// ���� ������ ���� ����
	for (auto oderableQuestElem : OrderableQuestElem)
		oderableQuestElem->RemoveFromParent();
	for (auto progressQuestElem : ProgressQuestElem)
		progressQuestElem->RemoveFromParent();
	OrderableQuestElem.Empty();
	ProgressQuestElem.Empty();


	// ���� ������ ����Ʈ ǥ��
	for (auto orderableQuestInfo : OrderableQuests)
	{
		UNpcQuestListElem * questElemWidget = CreateWidget<UNpcQuestListElem>(this, BP_NpcQuestListElem);

		OrderableQuestElem.Add(questElemWidget);
		ScrollBox_QuestList->AddChild(questElemWidget);

		questElemWidget->InitializeQuestListElem(
		/*npcQuestListWnd = */	this, 
		/*questCode       = */	orderableQuestInfo.Key,
		/*questInfo       = */	*orderableQuestInfo.Value,
		/*bIsProgress     = */  false);
	}

	// �������� ����Ʈ ǥ��
	for (auto progressQuestInfo : QuestInProgress)
	{
		UNpcQuestListElem* questElemWidget = CreateWidget<UNpcQuestListElem>(this, BP_NpcQuestListElem);
		ScrollBox_QuestList->AddChild(questElemWidget);

		ProgressQuestElem.Add(questElemWidget);

		questElemWidget->InitializeQuestListElem(
			/*npcQuestListWnd = */	this,
			/*questCode       = */	progressQuestInfo.Key,
			/*questInfo       = */	*progressQuestInfo.Value,
			/*bIsProgress     = */  true);

	}

}
