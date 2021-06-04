#include "ZoomableSpringArmComponent.h"

UZoomableSpringArmComponent::UZoomableSpringArmComponent()
{
	// �⺻������ �� ����� ����ϵ��� �մϴ�.
	bUseZoom = true;

	// �� �ּ�, �ִ� �Ÿ��� �����մϴ�.
	SetArmLengthMinMax(100.0f, 400.0f);
}

void UZoomableSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	// ��ǥ �Ÿ��� �����մϴ�.
	TargetArmLength = ArmLengthTarget =
		(MaxArmLength - MinArmLength) * 0.5f;
}

void UZoomableSpringArmComponent::TickComponent(float dt, 
	ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(dt, tickType, thisTickFunction);

	SmoothZoomSpringArm(dt);
}

void UZoomableSpringArmComponent::SmoothZoomSpringArm(float dt)
{
	TargetArmLength = FMath::FInterpTo(
		TargetArmLength, ArmLengthTarget,
		dt, 5.0f);
}

void UZoomableSpringArmComponent::ZoomCamera(float axis)
{
	// �� ����� ������� �ʴ´ٸ� ī�޶� ���� �̷������ ������ �մϴ�.
	if (!bUseZoom) return;

	ArmLengthTarget += (axis * -5.0f);
	ArmLengthTarget = FMath::Clamp(
		ArmLengthTarget, MinArmLength, MaxArmLength);

}
