#pragma once

#include "CoreMinimal.h"
#include "SkillProgressInfo.generated.h"


USTRUCT()
struct ARPG_API FSkillProgressInfo
{
	GENERATED_USTRUCT_BODY()

public:
	// ������ ��ų �ڵ�
	FName ProgressSkillCode;

	// ��ų ��Ÿ��
	float SkillCoolTime;

	// ��ų ���� �ð�
	float SkillCastTime;

	// ��ų �޺�
	int32 SkillCombo;

	// ���� ��ų ���� �ε���
	int CurrentSkillRangeIndex;

public :
	FSkillProgressInfo();
	FSkillProgressInfo(
		FName progressSkillCode, 
		float skillCoolTime, 
		float skillCastTime, 
		int32 skillCombo = 0, 
		int32 currentSkillRangeIndex = 0);

	// �޺� ī��Ʈ�� ������ŵ�ϴ�.
	FORCEINLINE void AddCombo()
	{ ++SkillCombo; }

	FORCEINLINE void ResetCombo()
	{ SkillCombo = 0; }

	// ��ų ���� �ε����� �ʱ�ȭ�մϴ�.
	FORCEINLINE void ResetSkillRangeIndex()
	{ CurrentSkillRangeIndex = 0; }

};
