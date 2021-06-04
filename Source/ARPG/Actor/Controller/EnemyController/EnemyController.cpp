#include "EnemyController.h"

#include "Enum/Team.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"



AEnemyController::AEnemyController()
{
	// AI Controller �� �ڱ� ���� ������Ʈ�� �����մϴ�.
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI_PERCEPTION")));
	/// - UAIPerceptionComponent : AI �� Ư���� �ڱ��� �����ϴ� ������Ʈ�� ��Ÿ���ϴ�.
	/// - �̷��� ������ �ڱ� ���� ������Ʈ�� GetPerceptionComponent() �� �̿��Ͽ� �ٽ� ���� �� �ֽ��ϴ�.
	
	// AI �� ���� �����մϴ�.
	SetGenericTeamId(FGenericTeamId(ETeam::Enemy));

	// Sight Sense
	{
		// �ð� �ڱ� ��ü ����
		AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_PERCEPTION"));

		// �þ� �Ÿ��� �����մϴ�.
		AISightConfig->SightRadius = 500.0f;

		// �þ� ��� �Ÿ��� �����մϴ�.
		AISightConfig->LoseSightRadius = 650.0f;

		// ���� ���� �þ߰��� �����մϴ�.
		AISightConfig->PeripheralVisionAngleDegrees = 180.0f;

		// �ڱ��� �Ҹ�Ǳ���� �ɸ��� �ð��� �����մϴ�.
		AISightConfig->SetMaxAge(2.0f);

		// ���� �����ǵ��� �մϴ�.
		AISightConfig->DetectionByAffiliation.bDetectEnemies = true;

		// ���� ���� �������� �ʵ��� �մϴ�.
		AISightConfig->DetectionByAffiliation.bDetectFriendlies = false;

		// �߸� ���� �������� �ʵ��� �մϴ�.
		AISightConfig->DetectionByAffiliation.bDetectNeutrals = false;

		// �ش� ������ ����ϴ� �ð� �ڱ� ��ü�� �����ŵ�ϴ�.
		GetPerceptionComponent()->ConfigureSense(*AISightConfig);

		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnSightUpdated);
	}

	TrackingTargetActor = nullptr;
}

bool AEnemyController::UpdateTrackingPosition()
{
	if (TrackingTargetActor == nullptr) return false;

	else
	{
		FVector lineTraceStartLoc = TrackingTargetActor->GetActorLocation();
		FVector lineTraceEndLoc = lineTraceStartLoc + (FVector::DownVector * 1000.0f);

		TArray<AActor*> hitActors;
		FHitResult hitResult;

		if (UKismetSystemLibrary::LineTraceSingleByProfile(
			GetWorld(),
			lineTraceStartLoc,
			lineTraceEndLoc,
			FName(TEXT("SpawnEnemy")),
			true,
			hitActors,
			EDrawDebugTrace::None,
			hitResult,
			true))
		{
			GetBlackboardComponent()->SetValueAsVector(
				TEXT("TargetPosition"), hitResult.Location);

			return true;
		}
		else return false;
	}

}

void AEnemyController::OnSightUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// �þ߿� ���Դٸ�
	if (Stimulus.WasSuccessfullySensed())
	{
		TrackingTargetActor = Actor;
	}
	// �þ߿��� ������ٸ�
	else
	{
		TrackingTargetActor = nullptr;
	}

}

