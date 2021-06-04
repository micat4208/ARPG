#include "PlayerCharacter.h"

#include "AnimInstance/PlayerCharacterAnimInst/PlayerCharacterAnimInst.h"

#include "Component/ZoomableSpringArm/ZoomableSpringArmComponent.h"
#include "Component/CharacterMovementHelper/CharacterMovementHelperComponent.h"
#include "Component/PlayerInteract/PlayerInteractComponent.h"
#include "Component/SkillController/SkillControllerComponent.h"

#include "Enum/Team.h"


APlayerCharacter::APlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_TEST_MESH(
		TEXT("SkeletalMesh'/Game/Resources/PlayerCharacter/GKnight/Meshes/SK_GothicKnight_VF.SK_GothicKnight_VF'"));
	if (SK_TEST_MESH.Succeeded()) GetMesh()->SetSkeletalMesh(SK_TEST_MESH.Object);

	static ConstructorHelpers::FClassFinder<UPlayerCharacterAnimInst> BP_PLAYER_CHARACTER_ANIM_INST(
		TEXT("AnimBlueprint'/Game/Blueprints/AnimInstance/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (BP_PLAYER_CHARACTER_ANIM_INST.Succeeded()) BP_PlayerCharacterAnimInst = BP_PLAYER_CHARACTER_ANIM_INST.Class;

	// ������Ʈ �߰�
	SkillController = CreateDefaultSubobject<USkillControllerComponent>(TEXT("SKILL_CONTROLLER"));
	CharacterMovementHelper = CreateDefaultSubobject<UCharacterMovementHelperComponent>(TEXT("MOVEMENT_HELPER"));
	SpringArm = CreateDefaultSubobject<UZoomableSpringArmComponent>(TEXT("SPRING_ARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetConstraintAspectRatio(true);

	HeadMesh = GetMesh();
	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HAIR_MESH"));
	BeardMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BEARD_MESH"));
	TopMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TOP_MESH"));
	BottomMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BOTTOM_MESH"));
	RGloveMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RGLOVE_MESH"));
	LGloveMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LGLOVE_MESH"));
	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SHOES_MESH"));
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON_MESH"));


	{
		Parts.Empty();
		Parts.Add(EPartsType::PT_Hair, HairMesh);
		Parts.Add(EPartsType::PT_Beard, BeardMesh);
		Parts.Add(EPartsType::PT_Head, HeadMesh);
		Parts.Add(EPartsType::PT_Top, TopMesh);
		Parts.Add(EPartsType::PT_Bottom, BottomMesh);
		Parts.Add(EPartsType::PT_RGlove, RGloveMesh);
		Parts.Add(EPartsType::PT_LGlove, LGloveMesh);
		Parts.Add(EPartsType::PT_Shoes, ShoesMesh);
		Parts.Add(EPartsType::PT_Weapon, WeaponMesh);

	}


	// Skeletal Mesh Settings...
	{
		BeardMesh->SetupAttachment(HeadMesh);
		TopMesh->SetupAttachment(HeadMesh);
		BottomMesh->SetupAttachment(HeadMesh);
		RGloveMesh->SetupAttachment(HeadMesh);
		LGloveMesh->SetupAttachment(HeadMesh);
		ShoesMesh->SetupAttachment(HeadMesh);
		WeaponMesh->SetupAttachment(HeadMesh);

		HairMesh->SetupAttachment(HeadMesh, FName(TEXT("HAIR")));
	}



	PlayerInteract = CreateDefaultSubobject<UPlayerInteractComponent>(TEXT("PLAYER_INTERACT"));

	// Spring Arm Component �� ��Ʈ ������Ʈ�� �߰��մϴ�.
	SpringArm->SetupAttachment(GetRootComponent());

	// Camera Component �� SpringArm ������Ʈ�� �߰��մϴ�.
	Camera->SetupAttachment(SpringArm);

	// ��Ʈ�ѷ��� ȸ������ SpringArm Component ȸ�������� ����մϴ�.
	SpringArm->bUsePawnControlRotation = true;

	// ��Ʈ�ѷ��� ȸ���� Yaw, Pitch ȸ���� ����մϴ�.
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritPitch = true;

	// SpringArm �������� �����մϴ�.
	SpringArm->TargetOffset = FVector::UpVector * 70.0f;

	// ĳ���� �⺻ ��ġ / ȸ�� ����
	GetMesh()->SetRelativeLocationAndRotation(
		FVector::DownVector * 88.0f,
		FRotator(0.0f, -90.0f, 0.0f));

	// �� ������ ȸ���� ��Ʈ�ѷ��� Yaw ȸ���� ������� �ʵ��� �մϴ�.
	bUseControllerRotationYaw = false;

	// �̵��ϴ� �������� ĳ���͸� ȸ����ŵ�ϴ�.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ȸ�� �ӵ��� �����մϴ�.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 450.0f, 0.0f);

	// �ִ� �ν��Ͻ� Ŭ���� ����
	//GetMesh()->SetAnimClass(BP_PlayerCharacterAnimInst);

	Tags.Add(PLAYER_ACTOR_TAG);

	// ���� �����մϴ�.
	SetGenericTeamId(ETeam::Player);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed,
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::RunKeyPressed);

	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released,
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::RunKeyReleased);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed,
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::JumpKeyPressed);

	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed,
		GetPlayerInteract(), &UPlayerInteractComponent::TryInteraction);
	
	PlayerInputComponent->BindAction(TEXT("RegularAttack"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::RegularAttack);


	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), 
		SpringArm, &UZoomableSpringArmComponent::ZoomCamera);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"),
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::InputHorizontal);

	PlayerInputComponent->BindAxis(TEXT("Vertical"),
		GetCharacterMovementHelper(), &UCharacterMovementHelperComponent::InputVertical);
}

void APlayerCharacter::InitializeMeshs()
{
	// HeadMesh �� ������ �ٸ� Skeletal Mesh ���� �޾Ƽ� �����ϵ��� �մϴ�.
	// HairMesh->SetMasterPoseComponent(HeadMesh);
	BeardMesh->SetMasterPoseComponent(HeadMesh);
	TopMesh->SetMasterPoseComponent(HeadMesh);
	BottomMesh->SetMasterPoseComponent(HeadMesh);
	RGloveMesh->SetMasterPoseComponent(HeadMesh);
	LGloveMesh->SetMasterPoseComponent(HeadMesh);
	ShoesMesh->SetMasterPoseComponent(HeadMesh);
	WeaponMesh->SetMasterPoseComponent(HeadMesh);

	HeadMesh->SetAnimClass(BP_PlayerCharacterAnimInst);
	// ���� BaseSkeletalMesh(HeadMesh) �� ����ϴ� Skeleton �� ����� ���
	// SetMasterPoseComponent �� ������ Mesh ���� ������ ����, 
	// �� (Slave->SetMasterPoseComponent(nullptr))
	// BaseSkeletalMesh �� Skeletal Mesh �ּ��� ������ ��
	// SetMasterPoseComponent �� �翬�� ���Ѿ� ��.
}

void APlayerCharacter::RegularAttack()
{
	SkillController->RequestSkill(FName(TEXT("1000")));
}