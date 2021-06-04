#include "EnemySpawnerComponent.h"

#include "Actor/EnemySpawnRegion/EnemySpawnRegion.h"
#include "Actor/Character/EnemyCharacter/EnemyCharacter.h"

#include "Struct/EnemySpawnRegionInfo/EnemySpawnRegionInfo.h"

UEnemySpawnerComponent::UEnemySpawnerComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_SPAWN_REGION_INFO(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EnemySpawnRegionInfo.DT_EnemySpawnRegionInfo'"));
	if (DT_ENEMY_SPAWN_REGION_INFO.Succeeded()) DT_EnemySpawnRegionInfo = DT_ENEMY_SPAWN_REGION_INFO.Object;

	BoxExtent *= 10.0f;

	LineTraceStartHeight = BoxExtent.Z;
	/// - BoxExtent : ����ü ũ��(X, Y, Z) / 2 �� ���� ��Ÿ���ϴ�.

	SpawnPointXCount = SpawnPointYCount = 4;
	CurrentEnemyCount = 0;

	SpawnRegionInfo = nullptr;
}

void UEnemySpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �ڵ带 ����ϴ�.
	FString contextString;
	FName regionCode = Cast<AEnemySpawnRegion>(GetOwner())->GetRegionCode();
	SpawnRegionInfo = DT_EnemySpawnRegionInfo->FindRow<FEnemySpawnRegionInfo>(regionCode, contextString);

	if (SpawnRegionInfo == nullptr)
	{
		UE_LOG(LogTemp, Error,
			TEXT("EnemySpawnerComponent.cpp :: %d LINE :: SpawnRegionInfo is nullptr! (regionCode was %s)"),
			__LINE__, *regionCode.ToString());
		return;
	}

	// ���� ������ �ʱ�ȭ�մϴ�.
	InitializeSpawnPoints();


	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUObject(this, &UEnemySpawnerComponent::RandomSpawn);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 1.0f, true);
	RandomSpawn();
}

void UEnemySpawnerComponent::InitializeSpawnPoints()
{

	// �׸��带 �̷�� ���� �簢������ ũ�⸦ ����ϴ�.
	float gridYSize = ((BoxExtent.Y * 2) / SpawnPointYCount);
	float gridXSize = ((BoxExtent.X * 2) / SpawnPointXCount);

	FVector pivot = (GetComponentLocation() - BoxExtent);
	pivot.Z = 0.0f;

	for (int32 y = 0; y < SpawnPointYCount; ++y)
	{
		TArray<FVector> points;

		for (int32 x = 0; x < SpawnPointXCount; ++x)
		{
			// ���� �������� ���� ���� �簢������ �߽� ��ġ�� ����ϴ�.
			FVector spawnPoint = pivot +
				(GetOwner()->GetActorForwardVector() * ((gridXSize * x) + (gridXSize * 0.5f))) +
				(GetOwner()->GetActorRightVector() * ((gridYSize * y) + (gridYSize * 0.5f)));
			
			points.Add(spawnPoint);
		}

		SpawnPoints.Add(points);
	}
}

void UEnemySpawnerComponent::RandomSpawn()
{
	if (SpawnRegionInfo == nullptr) return;
	if (SpawnRegionInfo->EnemyCode.Num() == 0) return;
	if (SpawnRegionInfo->MaxEnemyCount == CurrentEnemyCount) return;


	int32 randomEnemyIndex = FMath::RandRange(
		0, SpawnRegionInfo->EnemyCode.Num() - 1);

	// �� ����
	SpawnEnemy(SpawnRegionInfo->EnemyCode[randomEnemyIndex]);
	++CurrentEnemyCount;
}

void UEnemySpawnerComponent::SpawnEnemy(FName enemyCode)
{
	// ������ų ������ ��� ���� ���� Ʈ���̽��� ���۽�ų ��ġ�� ������ ����
	FVector lineTraceStartLocation;

	// ����Ʈ���̽� ���� ��ġ�� ����ϴ�.
	lineTraceStartLocation = GetRandomSpawnablePoint();

	// ����Ʈ���̽� ���� ���̸� �����մϴ�.
	lineTraceStartLocation.Z = GetOwner()->GetActorLocation().Z + LineTraceStartHeight;

	// ����Ʈ���̽� ���� ���͸� ������ �迭
	TArray<AActor*> hitActors;

	// ����Ʈ���̽� ���� ����� ������ ����
	FHitResult hitResult;

	if (UKismetSystemLibrary::LineTraceSingleByProfile(
		GetWorld(),
		lineTraceStartLocation,
		lineTraceStartLocation + (FVector::DownVector * (LineTraceStartHeight * 2.0f)),
		FName(TEXT("SpawnEnemy")),
		true,
		hitActors,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		10.0f))
	{
		// ����Ʈ���̽� ��� ��ġ�� �����մϴ�.
		FVector spawnLocation = hitResult.Location + (FVector::UpVector * (LineTraceStartHeight * 2.0f));

		// �� ĳ���� ����
		AEnemyCharacter* newEnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(
			spawnLocation, FRotator::ZeroRotator);
		newEnemyCharacter->InitializeEnemyCharacter(enemyCode);
	}
}

FVector UEnemySpawnerComponent::GetRandomSpawnablePoint() const
{
	int32 randomYIndex = FMath::RandRange(0, SpawnPointYCount - 1);
	int32 randomXIndex = FMath::RandRange(0, SpawnPointXCount - 1);

	return SpawnPoints[randomYIndex][randomXIndex];
}
