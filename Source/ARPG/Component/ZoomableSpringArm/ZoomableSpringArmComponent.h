#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ZoomableSpringArmComponent.generated.h"

UCLASS()
class ARPG_API UZoomableSpringArmComponent final : 
	public USpringArmComponent
{
	GENERATED_BODY()

private :
	bool bUseZoom;

	float ArmLengthTarget;
	float MinArmLength;
	float MaxArmLength;

public :
	UZoomableSpringArmComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float dt, 
		enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private :
	// �ε巴�� ���� �̷������ �ֵ��� �մϴ�.
	void SmoothZoomSpringArm(float dt);

public :
	// ī�޶� ���� �����մϴ�.
	/// - APlayerCharacter ���� ���콺 �� �Է¿� ���ε��˴ϴ�.
	void ZoomCamera(float axis);

public :
	FORCEINLINE void SetUseZoom(bool bUse)
	{ bUseZoom = bUse; }

	// ���� ������ �ּ�, �ִ� �Ÿ��� �����մϴ�.
	FORCEINLINE void SetArmLengthMinMax(float min, float max)
	{
		MinArmLength = min;
		MaxArmLength = max;
	}
	
};
