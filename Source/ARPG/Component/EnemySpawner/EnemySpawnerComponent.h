#pragma once

#include "ARPG.h"
#include "Components/BoxComponent.h"
#include "EnemySpawnerComponent.generated.h"

UCLASS()
class ARPG_API UEnemySpawnerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
private :
	class UDataTable* DT_EnemySpawnRegionInfo;



private :
	// ���� ������ X �� �������� � ��ġ�ϵ��� �� �������� �����մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region Settings", meta = (AllowPrivateAccess = "true"))
	int32 SpawnPointXCount;

	// ���� ������ Y �� �������� � ��ġ�ϵ��� �� �������� �����մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region Settings", meta = (AllowPrivateAccess = "true"))
	int32 SpawnPointYCount;

	// ����Ʈ���̽� ���� ���̸� �����մϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Region Settings", meta = (AllowPrivateAccess = "true"))
	float LineTraceStartHeight;

	// �� ���� ���� ������ ��Ÿ���ϴ�.
	struct FEnemySpawnRegionInfo* SpawnRegionInfo;

	// ���� �������� ��Ÿ���ϴ�.
	TArray<TArray<FVector>> SpawnPoints;

	// ���� ������ �� ��ü��
	int32 CurrentEnemyCount;

public :
	UEnemySpawnerComponent();

	virtual void BeginPlay() override;

private :
	// ���� �������� �ʱ�ȭ�մϴ�.
	void InitializeSpawnPoints();

	void RandomSpawn();

	void SpawnEnemy(FName enemyCode);

	// ������ ���� ��ġ�� ����ϴ�.
	FVector GetRandomSpawnablePoint() const;



};
