#pragma once


// �ൿ Ÿ���� ��Ÿ���ϴ�.
/// - �� ���� ���Ŀ� ��Ҹ� �߰��ϴ� ���
///   UPlayerBehaviorBroadcastComponent ���� �߰��� �ൿ�� ��ġ�ϴ� 
///   PlayerBehaviorEvents ��Ҹ� �߰��ؾ� �մϴ�.
namespace EPlayerBehavior
{
	enum Type : uint8
	{
		// Default
		BH_None,

		// Item Event
		BH_GetItem,

		// Quest Event
		BH_StartQuest,
		BH_FinishQuest
	};
}