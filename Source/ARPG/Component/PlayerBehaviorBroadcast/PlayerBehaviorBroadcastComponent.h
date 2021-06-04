#pragma once

#include "ARPG.h"
#include "Components/ActorComponent.h"

#include "Struct/PlayerBehaviorData/PlayerBehaviorData.h"

#include "PlayerBehaviorBroadcastComponent.generated.h"


DECLARE_EVENT_OneParam(UPlayerBehaviorBroadcastComponent, FPlayerBehaviorEvent, FPlayerBehaviorData&)
// �̺�Ʈ
/// - �ش� Ŭ���� �������� ȣ���� �� �ִ� �븮���Դϴ�.
/// - �⺻������ MULTICAST �Դϴ�.
/// - ���� ���� ��� DECLARE_EVENT(�̺�Ʈ ��� Ŭ������, �̺�Ʈ ����, �Ű� ���� Ÿ�Ե�...)



UCLASS()
class ARPG_API UPlayerBehaviorBroadcastComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	// Ư���� �ൿ�� ���� �� ȣ��Ǵ� �븮�ڵ��� ��Ÿ���ϴ�.
	TMap<EPlayerBehavior::Type, FPlayerBehaviorEvent> PlayerBehaviorEvents;

	// �ൿ �����Ͱ� ����˴ϴ�.
	TQueue<FPlayerBehaviorData> PlayerBehaviors;

public:	
	UPlayerBehaviorBroadcastComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// �߻��� �ൿ�� ó���մϴ�.
	void BehaviorProcedure();

public :
	// �ൿ�� �߰��մϴ�.
	void AddBehavior(FPlayerBehaviorData newBehaviorData);




};
