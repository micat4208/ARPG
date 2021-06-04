#pragma once

#include "ARPG.h"
#include "Actor/Character/BaseCharacter.h"
#include "Struct/EnemyData/EnemyData.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ARPG_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private :
	class UDataTable* DT_EnemyData;
	TSubclassOf<class UEnemyCharacterWidget> CharacterWidgetClass;

private :
	class UBehaviorTree* BehaviorTree;

protected :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCharacterWidgetComponent* CharacterWidget;

public :
	// �� ĳ���� �ڵ带 ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Code")
	FName EnemyCode;

	// �� ĳ���� ������ ��Ÿ���ϴ�.
	FEnemyData * EnemyData;

public :
	AEnemyCharacter();

public :
	virtual void PossessedBy(AController* NewController) override;

public :
	// �� ĳ���͸� �ʱ�ȭ�մϴ�.
	/// - �� �޼���� BeginPlay() �� ȣ��� �� ȣ��˴ϴ�.
	void InitializeEnemyCharacter(FName enemyCode);

public :
	FORCEINLINE class UCharacterWidgetComponent* GetCharacterWidget() const
	{ return CharacterWidget; }
	


	
};
