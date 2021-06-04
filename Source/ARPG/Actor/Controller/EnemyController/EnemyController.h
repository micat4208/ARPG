#pragma once

#include "ARPG.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

UCLASS()
class ARPG_API AEnemyController : 
	public AAIController
{
	GENERATED_BODY()

private :
	// �ڱ� ���� ��ü���� ���� �ð� �ڱ� ��ü�� ��Ÿ���ϴ�.
	class UAISenseConfig_Sight* AISightConfig;

	// ���� ��ǥ ���͸� ��Ÿ���ϴ�.
	AActor* TrackingTargetActor;


public :
	AEnemyController();

	// ���� ��ġ�� �����մϴ�.
	/// - TrackingTargetActor �� nullptr �� ��� false �� ��ȯ�մϴ�.
	bool UpdateTrackingPosition();

private :
	// �ð� ������ ������ ������ ���°� ���Ӱ� ���ŵ� ��� ȣ��� �޼����Դϴ�.
	UFUNCTION()
	void OnSightUpdated(AActor* Actor, FAIStimulus Stimulus);
	/// - Actor : ���°� ���ŵ� ���Ͱ� ���޵˴ϴ�.
	/// - Stimulus : ���� ���� ����� ���� �������� �������� ��Ÿ���ϴ�.

	
};
