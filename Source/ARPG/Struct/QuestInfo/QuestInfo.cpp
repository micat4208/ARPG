#include "QuestInfo.h"

#include "Single/PlayerManager/PlayerManager.h"

bool FQuestInfo::IsProgress(UPlayerManager* playerManager, FName questCode)
{
	FPlayerCharacterInfo* playerCharacterInfo = playerManager->GetPlayerInfo();

	for (auto info : playerCharacterInfo->ProgressQuestInfos)
		if (info.QuestCode == questCode) return true;

	return false;
}

bool FQuestInfo::IsOrderable(UPlayerManager* playerManager, FName questCode)
{
	FPlayerCharacterInfo* playerCharacterInfo = playerManager->GetPlayerInfo();

	// ���� ������ ĳ���� Ŭ�������� Ȯ���մϴ�.
	if (QuestAvailableClass != ECharacterClassType::CLS_ALL &&
		QuestAvailableClass != playerCharacterInfo->CharacterClass)
		return false;

	// ���� Ȯ��
	if (QuestAvailableLevel > playerCharacterInfo->CharacterLevel)
		return false;

	// �������� ����Ʈ���� Ȯ��
	for (auto questInfo : playerCharacterInfo->ProgressQuestInfos)
		// �������� ����Ʈ��� ���� �Ұ���
		if (questCode == questInfo.QuestCode) return false;

	// ���� ����Ʈ �Ϸ� Ȯ��
	for (auto questCode : PrecedentQuestList)
	{
		// ���� ����Ʈ �̿ϷḦ ��Ÿ���ϴ�.
		bool incompleted = true;

		for (auto completedQuestCode : playerCharacterInfo->CompletedQuestCodes)
		{
			if (questCode == completedQuestCode)
			{
				// ���� ����Ʈ �Ϸ���
				incompleted = false;
				break;
			}
		}

		// ���� ����Ʈ(questCode)�� �Ϸ����� �ʾҴٸ� ���� �Ұ���
		if (incompleted) return false;
	}

	return true;
}
