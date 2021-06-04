#include "PlayerBehaviorBroadcastComponent.h"

UPlayerBehaviorBroadcastComponent::UPlayerBehaviorBroadcastComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerBehaviorBroadcastComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerBehaviorEvents.Add(BH_None, FPlayerBehaviorEvent());
	PlayerBehaviorEvents.Add(BH_GetItem, FPlayerBehaviorEvent());
	PlayerBehaviorEvents.Add(BH_StartQuest, FPlayerBehaviorEvent());
	PlayerBehaviorEvents.Add(BH_FinishQuest, FPlayerBehaviorEvent());
}

void UPlayerBehaviorBroadcastComponent::TickComponent(
	float DeltaTime, 
	ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BehaviorProcedure();
}

void UPlayerBehaviorBroadcastComponent::BehaviorProcedure()
{
	// �߻��� �ൿ�� �������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (PlayerBehaviors.IsEmpty()) return;

	// �߰��� �ൿ�� ����ϴ�.
	FPlayerBehaviorData newBehavior;
	PlayerBehaviors.Dequeue(newBehavior);

	// �ൿ Ÿ�Կ� ���� �̺�Ʈ�� �����ŵ�ϴ�.
	PlayerBehaviorEvents[newBehavior.Behavior].Broadcast(newBehavior);
}

void UPlayerBehaviorBroadcastComponent::AddBehavior(FPlayerBehaviorData newBehaviorData)
{
	PlayerBehaviors.Enqueue(newBehaviorData);
}

