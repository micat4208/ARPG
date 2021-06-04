#include "SkillControllerComponent.h"

#include "Single/GameInstance/RPGGameInst.h"

#include "Actor/Level/BaseLevelScriptActor.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"


USkillControllerComponent::USkillControllerComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SKILL_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_SkillInfo.DT_SkillInfo'"));
	if (DT_SKILL_INFO.Succeeded()) DT_SkillInfo = DT_SKILL_INFO.Object;

	PrimaryComponentTick.bCanEverTick = true;

	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	SkillCount = 0;
	bIsRequestable = true;
	CurrentSkillInfo = PrevSkillInfo = nullptr;

}

void USkillControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLevel = Cast<ABaseLevelScriptActor>(GetWorld()->GetLevelScriptActor());
}

void USkillControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SkillProcedure();
}

void USkillControllerComponent::SkillProcedure()
{
	// ���� �������� ��ų�� �����Ѵٸ� return
	/// - ��ų�� ���������� �ϳ��� ����ǵ��� �մϴ�.
	if (CurrentSkillInfo != nullptr) return;

	// ť�� ��Ұ� �������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (SkillQueue.IsEmpty()) return;

	// ������ �����ų ��ų ������ ����ϴ�.
	FSkillInfo* skillInfo;
	SkillQueue.Dequeue(skillInfo);

	--SkillCount;

	// ��ų ���� ����
	UpdateUsedSkillInfo(skillInfo);

	// ��ų�� �����մϴ�.
	CastSkill(skillInfo);
}

void USkillControllerComponent::UpdateUsedSkillInfo(FSkillInfo* newSkillInfo)
{
	// ���� ����� ��ų�� ���� ���� ��ų�� ���� ��ų�� �ƴ϶��, ���� ��ų�� ������ ���Ǿ��� �� 
	// ���� ���� ������ ���·� �����մϴ�.
	if (PrevSkillInfo != nullptr)
	{
		if (PrevSkillInfo->SkillCode != newSkillInfo->SkillCode)
		{
			UsedSkillInfo[PrevSkillInfo->SkillCode].SkillCombo = -1;
		}
	}

	// ������ ��ų�� ���� ���� �ִٸ�
	if (UsedSkillInfo.Contains(newSkillInfo->SkillCode))
	{
		// �޺��� ����ϴ� ��ų�̶��
		if (newSkillInfo->GetMaxComboCount() != 0)
		{
			// �޺� ī��Ʈ�� ������ŵ�ϴ�.
			UsedSkillInfo[newSkillInfo->SkillCode].AddCombo();

			// �޺� ī��Ʈ�� �ִ� �޺� ī��Ʈ�� �ʰ��Ѵٸ� ���½�ŵ�ϴ�.
			if (UsedSkillInfo[newSkillInfo->SkillCode].SkillCombo == newSkillInfo->GetMaxComboCount())
			{
				UsedSkillInfo[newSkillInfo->SkillCode].ResetCombo();

				// ���� ����ߴ� ��ų ���� �ε����� �ʱ�ȭ�մϴ�.
				UsedSkillInfo[PrevSkillInfo->SkillCode].ResetSkillRangeIndex();
			}

		}

	}
	// ��ų�� ó�� ���ȴٸ� ���ο� �����͸� �߰��մϴ�.
	else 
	{
		FSkillProgressInfo newSkillProgressInfo = FSkillProgressInfo(
			newSkillInfo->SkillCode,
			0.0f, 0.0f,
			0);

		UsedSkillInfo.Add(newSkillInfo->SkillCode, newSkillProgressInfo);
	}
}

void USkillControllerComponent::CastSkill(FSkillInfo* skillInfo)
{
	// ���ο� ��ų�� ���� ��� ��ų�� �����մϴ�.
	CurrentSkillInfo = skillInfo;

	// ��ų ��û �Ұ��� ���·� �����մϴ�.
	bIsRequestable = false;






	// �ִϸ��̼��� ����մϴ�.
	UAnimMontage* skillAnimMontage = Cast<UAnimMontage>(
		GetManager(FStreamableManager)->LoadSynchronous(CurrentSkillInfo->AnimMontagePath));

	// ������ ����� ��ų�� �����Ѵٸ�
	if (PrevSkillInfo != nullptr)
	{
		// ���� ��ų�� ���� �������� ����ߴٸ�
		if ((PrevSkillInfo->SkillCode == CurrentSkillInfo->SkillCode) &&
			CurrentSkillInfo->LinkableSkillSectionNames.Num() != 0)
		{
			// �ִ� ��Ÿ�� ���� �̸��� ����ϴ�.
			int32 currentComboCount = UsedSkillInfo[CurrentSkillInfo->SkillCode].SkillCombo;
			FName sectionName = CurrentSkillInfo->LinkableSkillSectionNames[currentComboCount];

			// ������ ������ �ִϸ��̼��� ����մϴ�.
			PlayerCharacter->PlayAnimMontage(skillAnimMontage, 1.0f, sectionName);
			return;
		}
		// �ٸ� ��ų�� ����ߴٸ�
		else 
		{
			// ������ ����� ��ų�� ������ ���Ǿ��� �� ���� ���� ������ ���·� �����մϴ�.
			UsedSkillInfo[PrevSkillInfo->SkillCode].SkillCombo = -1;
		}
	}

	PlayerCharacter->PlayAnimMontage(skillAnimMontage);
}

void USkillControllerComponent::MakeSkillRange(
	FSkillInfo* skillInfo,
	FVector tracingStart, 
	FVector tracingEnd, 
	float radius, 
	FName profileName)
{
	// ���� ���� ��� ���͵��� ��Ÿ���ϴ�.
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetOwner());

	// ���� ����� ���� �迭�� ��Ÿ���ϴ�.
	TArray<FHitResult> hitResults;


	UKismetSystemLibrary::SphereTraceMultiByProfile(
		GetWorld(),
		tracingStart,
		tracingEnd,
		radius,
		profileName,
		true,
		actorsToIgnore,
		EDrawDebugTrace::Type::ForDuration,
		hitResults,
		true);

	for (FHitResult hitresult : hitResults)
	{
		CurrentLevel->CreatePoolableParticleActor(
			Cast<UParticleSystem>(GetManager(FStreamableManager)->LoadSynchronous(skillInfo->HitParticle)),
			hitresult.ImpactPoint);
	}
}

void USkillControllerComponent::RequestSkill(FName skillCode)
{
	// ��ų�� ��û�� �� ���� ��� �������� �ʽ��ϴ�.
	if (!bIsRequestable) return;

	// ��ų�� 3�� �̻� ��ϵƴٸ� �����Ű�� �ʽ��ϴ�.
	if (SkillCount >= 3) return;

	// ��û�� ��ų ������ ����ϴ�.
	FString contextString;
	FSkillInfo* requestSkillInfo = DT_SkillInfo->FindRow<FSkillInfo>(skillCode, contextString);

	// ��û�� ��ų�� ã�� ������ ��� ��û ���
	if (requestSkillInfo == nullptr) return;

	// �����ų ��ų�� ��� ť�� �߰��մϴ�.
	SkillQueue.Enqueue(requestSkillInfo);
	++SkillCount;
}

void USkillControllerComponent::SkillFinished()
{
	// ���� ��ų�� ���� ��ų ������ �����մϴ�.
	PrevSkillInfo = CurrentSkillInfo;
	CurrentSkillInfo = nullptr;

	// ��ų ��û ���� ���·� �����մϴ�.
	bIsRequestable = true;

	// ����� ��ų�� �������� �ʴ´ٸ�
	if (SkillCount == 0)
	{
		// ������ ����� ��ų�� ������ ���Ǿ��� �� ���� ���� ������ ���·� �����մϴ�.
		UsedSkillInfo[PrevSkillInfo->SkillCode].SkillCombo = -1;

		// ����ߴ� ��ų ���� �ε����� �ʱ�ȭ�մϴ�.
		UsedSkillInfo[PrevSkillInfo->SkillCode].ResetSkillRangeIndex();
	}
}

void USkillControllerComponent::NextSkillRangeIndex()
{
	// ���� �������� ��ų�� �������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (CurrentSkillInfo == nullptr) return;

	// ��ų ���� �ε��� ����
	++UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex;

	// ���� �ε����� �迭 ������ �ʰ��Ѵٸ� ������ ��Ҹ� ����ϵ��� �մϴ�.
	if (UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex ==
		CurrentSkillInfo->SkillRangeInfos.Num())
		--UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex;
}

void USkillControllerComponent::MakeSkillRange()
{
	int32 currentSkillRangeIndex = UsedSkillInfo[CurrentSkillInfo->SkillCode].CurrentSkillRangeIndex;

	// ��ų ���� ������ ����ϴ�.
	FSkillRangeInfo skillRangeInfo = CurrentSkillInfo->SkillRangeInfos[currentSkillRangeIndex];
	TTuple<int32, float> calcFormulaRes = skillRangeInfo.GetSkillCalcFormulaResult();
	int32 rangeCount = calcFormulaRes.Get<0>();
	float value = calcFormulaRes.Get<1>();

	
	LOG(TEXT("form = %s"), *skillRangeInfo.DamageCalcFormula);

	// Sphere Tracing �� ������� �ʴ´ٸ� �������� �ʽ��ϴ�.
	if (!skillRangeInfo.bUseSphereTrace) return;

	// Ʈ���̽� �������� ����ϴ�.
	FVector tracingOffset =
		(GetOwner()->GetActorForwardVector() * skillRangeInfo.TraceStartOffset.X) +
		(GetOwner()->GetActorRightVector() * skillRangeInfo.TraceStartOffset.Y) +
		(GetOwner()->GetActorUpVector() * skillRangeInfo.TraceStartOffset.Z);

	// Ʈ���̽� ���� ��ġ�� ��Ÿ���ϴ�.
	FVector tracingStart = 
		GetOwner()->GetActorLocation() + tracingOffset;

	// Ʈ���̽� ����
	FVector tracingDirection = skillRangeInfo.GetTraceDirection(GetOwner());

	// Ʈ���̽� �� ��ġ�� ��Ÿ���ϴ�.
	FVector tracingEnd = tracingStart + (tracingDirection * skillRangeInfo.TraceDistance);

	// ����ϴ� ��ų ������ ��Ÿ���ϴ�.
	FSkillInfo* skillInfo = CurrentSkillInfo;

	for (int32 i = 0; i < rangeCount; ++i)
	{
		if (FMath::IsNearlyZero(skillRangeInfo.CreateDelay) || (i == 0))
			MakeSkillRange(skillInfo, tracingStart, tracingEnd, skillRangeInfo.TraceRadius, FName(TEXT("PlayerSkill")));
		else
		{
			FTimerHandle timerHandle;
			FTimerDelegate timerDelegate;
			timerDelegate.BindLambda([skillInfo, tracingStart, tracingEnd, skillRangeInfo, this]()
				{
					MakeSkillRange(skillInfo, tracingStart, tracingEnd, skillRangeInfo.TraceRadius, FName(TEXT("PlayerSkill")));
				});

			// Ÿ�̸� ����
			GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, skillRangeInfo.CreateDelay * i, false);

		}
	}
}
