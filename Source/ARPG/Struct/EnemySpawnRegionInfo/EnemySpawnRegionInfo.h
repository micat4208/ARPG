#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnRegionInfo.generated.h"

// �� ���� ���� ������ ��Ÿ���ϴ�.
USTRUCT(BlueprintType)
struct ARPG_API FEnemySpawnRegionInfo : 
	public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxEnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> EnemyCode;

public :
	FEnemySpawnRegionInfo();

};
