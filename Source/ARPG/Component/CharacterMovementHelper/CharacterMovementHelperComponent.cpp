#include "CharacterMovementHelperComponent.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

UCharacterMovementHelperComponent::UCharacterMovementHelperComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WalkSpeed = 300.0f;
	RunSpeed = 600.0f;
}

void UCharacterMovementHelperComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// ���߿��� ĳ���͸� 80% ���� �����ϵ��� �մϴ�.
	PlayerCharacter->GetCharacterMovement()->AirControl = 0.8f;

	// ĳ���Ͱ� �޴� �߷� ����
	PlayerCharacter->GetCharacterMovement()->GravityScale = 2.5f;

	// ���� �ʱ� ���ӷ� ����
	PlayerCharacter->GetCharacterMovement()->JumpZVelocity = 1000.0f;
}

void UCharacterMovementHelperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCharacterMovementHelperComponent::IsInAir() const
{
	return IsValid(PlayerCharacter) ? 
		PlayerCharacter->GetCharacterMovement()->IsFalling() : true;
}

void UCharacterMovementHelperComponent::RunKeyPressed()
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void UCharacterMovementHelperComponent::RunKeyReleased()
{
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UCharacterMovementHelperComponent::JumpKeyPressed()
{
	PlayerCharacter->Jump();
}

void UCharacterMovementHelperComponent::InputHorizontal(float axis)
{
	// ��Ʈ�ѷ� ȸ���� Yaw ȸ������ �����մϴ�.
	FRotator yawRotation(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f);

	// ��Ʈ�ѷ� ���� ������ ������ �����մϴ�.
	FVector rightVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	// FRotationMatrix : ȸ���� ��ǥ�� ������ �����ϴ� ����� ��Ÿ���ϴ�.
	// GetUnitAxis(EAxis) : EAxis �������� ������ ��ȯ�մϴ�.

	PlayerCharacter->AddMovementInput(rightVector, axis);
}

void UCharacterMovementHelperComponent::InputVertical(float axis)
{
	// ��Ʈ�ѷ� ȸ���� Yaw ȸ�� ���
	FRotator yawRotation(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f);

	// ��Ʈ�ѷ� ���� �� ���� ����
	FVector forwardVector = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	PlayerCharacter->AddMovementInput(forwardVector, axis);
}

