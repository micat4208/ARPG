#include "EnemyCharacter.h"

#include "Single/GameInstance/RPGGameInst.h"

#include "Actor/Controller/EnemyController/EnemyController.h"

#include "Component/CharacterWidget/CharacterWidgetComponent.h"

#include "Widget/CharacterWidget/EnemyCharacterWidget/EnemyCharacterWidget.h"

#include "BehaviorTree/BehaviorTree.h"

AEnemyCharacter::AEnemyCharacter()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_DATA(
		TEXT("DataTable'/Game/Resources/DataTables/DT_EnemyData.DT_EnemyData'"));
	if (DT_ENEMY_DATA.Succeeded()) DT_EnemyData = DT_ENEMY_DATA.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MINO(
		TEXT("SkeletalMesh'/Game/Resources/Enemies/Mino/Mesh/SK_Mino.SK_Mino'"));
	if (SK_MINO.Succeeded()) GetMesh()->SetSkeletalMesh(SK_MINO.Object);

	static ConstructorHelpers::FClassFinder<UEnemyCharacterWidget> BP_ENEMY_CHARACTER_WIDGET(
		TEXT("WidgetBlueprint'/Game/Blueprints/Widget/CharacterWidget/BP_EnemyCharacterWidget.BP_EnemyCharacterWidget_C'"));
	if (BP_ENEMY_CHARACTER_WIDGET.Succeeded()) CharacterWidgetClass = BP_ENEMY_CHARACTER_WIDGET.Class;

	CharacterWidget = CreateDefaultSubobject<UCharacterWidgetComponent>(TEXT("CHARACTER_WIDGET"));
	CharacterWidget->SetupAttachment(GetRootComponent());

	// ����ϴ� AI Controller �� �����մϴ�.
	AIControllerClass = AEnemyController::StaticClass();

	// ��Ʈ�ѷ� ���� ����� �����մϴ�.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BehaviorTree = nullptr;

	// ���� �����մϴ�.
	SetGenericTeamId(ETeam::Enemy);
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);



}


void AEnemyCharacter::InitializeEnemyCharacter(FName enemyCode)
{
	EnemyCode = enemyCode;

	// �� �ڵ带 �̿��Ͽ� �� ������ ����ϴ�.
	FString contextString;
	EnemyData = DT_EnemyData->FindRow<FEnemyData>(EnemyCode, contextString);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	/// - SetCollisionProfileName(InCollisionProfileName) : �ø��� ��������(Collision Preset)�� �����մϴ�.


	// �� ������ ���������� ���� ���ߴٸ�
	if (EnemyData == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCharacter.cpp :: %d LINE :: EnemyData is null (EnemyCode Is %s)"),
			__LINE__, *EnemyCode.ToString());
		return;
	}

	// ĳ���� ���� �ʱ�ȭ
	CharacterWidget->InitializeCharacterWidget(CharacterWidgetClass, FVector2D(150.0f, 60.0f));
	CharacterWidget->SetWidgetHeight((EnemyData->CapsuleHalfHeight * 2.0f) + 10.0f);
	CharacterWidget->GetCharacterWidget()->SetName(EnemyData->EnemyName);

	// �ִ� �̵� �ӷ� ����
	GetCharacterMovement()->MaxWalkSpeed = EnemyData->MaxMoveSpeed;

	// �� ĸ�� ũ�� ����
	GetCapsuleComponent()->SetCapsuleHalfHeight(EnemyData->CapsuleHalfHeight);
	GetCapsuleComponent()->SetCapsuleRadius(EnemyData->CapsuleRadius);

	// ���̷�Ż �޽� ����
	USkeletalMesh* skeletalMesh = Cast<USkeletalMesh>(GetManager(FStreamableManager)->LoadSynchronous(EnemyData->SkeletalMeshPath));
	/// - LoadSynchronous(target) : target �� �ش��ϴ� �ּ��� ���� ������� �ε��Ͽ� UObject* �������� ��ȯ�մϴ�.
	GetMesh()->SetSkeletalMesh(skeletalMesh);

	// ���̷�Ż �޽� ��ġ / ȸ�� ����
	GetMesh()->SetRelativeLocationAndRotation(
		FVector::DownVector * 88.0f,
		FRotator(0.0f, -90.0f, 0.0f));

	// BehaviorTree ����
	BehaviorTree = Cast<UBehaviorTree>(
		GetManager(FStreamableManager)->LoadSynchronous(EnemyData->UseBehaviorTreeAssetPath));

	if (IsValid(BehaviorTree))
		Cast<AEnemyController>(GetController())->RunBehaviorTree(BehaviorTree);

	// �ִ� �ν��Ͻ� Ŭ���� ����
	GetMesh()->SetAnimInstanceClass(EnemyData->AnimClass);

	
}
