#pragma once

#include "ARPG.h"

#include "Components/ActorComponent.h"

#include "Struct/SkillInfo/SkillInfo.h"
#include "Struct/SkillProgressInfo/SkillProgressInfo.h"

#include "SkillControllerComponent.generated.h"


UCLASS()
class ARPG_API USkillControllerComponent : 
	public UActorComponent
{
	GENERATED_BODY()

private :
	class UDataTable* DT_SkillInfo;

private :
	class ABaseLevelScriptActor* CurrentLevel;

	// �÷��̾� ĳ���͸� ��Ÿ���ϴ�.
	class APlayerCharacter* PlayerCharacter;

	// ���� �������� ��ų ������ ��Ÿ���ϴ�.
	FSkillInfo* CurrentSkillInfo;

	// �ٷ� ������ �����Ų ��ų ������ ��Ÿ���ϴ�.
	FSkillInfo* PrevSkillInfo;

	// �����߾��� ��ų ������ ��Ƶ� ��
	/// - �޺��� ��Ÿ���� ����ϱ� ���� ���˴ϴ�.
	TMap<FName, FSkillProgressInfo> UsedSkillInfo;

	// �����ų ��ų���� ��Ƶ� ť
	TQueue<FSkillInfo*> SkillQueue;

	// �Էµ� ��ų ī��Ʈ�� ��Ÿ���ϴ�.
	int32 SkillCount;

	// ��ų�� ��û�� �� ������ ��Ÿ���ϴ�.
	/// - �� ���� false ��� ��ų ������ ��û�� �� �����ϴ�.
	bool bIsRequestable;

public:	
	USkillControllerComponent();

public:	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// ��ų�� ������� ó���մϴ�.
	void SkillProcedure();
	
	// ���� ��ų ���¸� �����մϴ�.
	void UpdateUsedSkillInfo(FSkillInfo* newSkillInfo);

	// ��ų�� �����մϴ�.
	void CastSkill(FSkillInfo* skillInfo);

	void MakeSkillRange(
		FSkillInfo * skillInfo,
		FVector tracingStart, 
		FVector tracingEnd, 
		float radius, 
		FName profileName);

public :
	// ��ų ������ ��û�մϴ�.
	/// - skillCode : ��û��ų ��ų �ڵ带 �����մϴ�.
	void RequestSkill(FName skillCode);

public :
	// ��ų ��û ���� ���¸� �����մϴ�.
	FORCEINLINE void SetSkillRequestable(bool bRequestable)
	{ bIsRequestable = bRequestable; }

	// ��ų�� �������� �˸��ϴ�.
	void SkillFinished();

	// ��ų ���� �ε����� ���� �ε����� �����մϴ�.
	void NextSkillRangeIndex();

	// ��ų ������ �����մϴ�.
	void MakeSkillRange();
		
};
