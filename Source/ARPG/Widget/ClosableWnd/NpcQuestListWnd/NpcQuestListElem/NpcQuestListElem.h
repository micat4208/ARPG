#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Struct/QuestInfo/QuestInfo.h"

#include "NpcQuestListElem.generated.h"

UCLASS()
class ARPG_API UNpcQuestListElem : public UUserWidget
{
	GENERATED_BODY()

private :
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Quest;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QuestType;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QuestTitle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_IsProgress;

private :
	// �ش� ������ �����ϴ� Npc ����Ʈ ��� â ������ ��Ÿ���ϴ�.
	class UNpcQuestListWnd* NpcQuestListWnd;

	// ����Ʈ �ڵ带 ��Ÿ���ϴ�.
	FName QuestCode;

	// ����Ʈ ������ ��Ÿ���ϴ�.
	FQuestInfo QuestInfo;

	// �������� �Ƿ����� ��Ÿ���ϴ�.
	bool bIsProgress;


public :
	// ����Ʈ ��� ��� ������ �ʱ�ȭ�մϴ�.
	/// - npcQuestListWnd : �ش� ������ �����ϴ� Npc ����Ʈ ��� â ������ �����մϴ�.
	/// - questCode : ����Ʈ �ڵ带 �����մϴ�.
	/// - questInfo : ����Ʈ ������ �����մϴ�.
	void InitializeQuestListElem(
		class UNpcQuestListWnd* npcQuestListWnd,
		FName questCode,
		FQuestInfo questInfo,
		bool bIsProgress);

private :
	UFUNCTION()
	void OnQuestButtonClicked();
	
};
