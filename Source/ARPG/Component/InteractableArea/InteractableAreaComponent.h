#pragma once

#include "ARPG.h"
#include "Components/SphereComponent.h"
#include "InteractableAreaComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FInteractEvent, class UInteractableAreaComponent *)

UCLASS()
class ARPG_API UInteractableAreaComponent : public USphereComponent
{
	GENERATED_BODY()
	
public :
	// ��ȣ�ۿ��� ���۵Ǿ��� �� ȣ��Ǵ� �븮��
	FInteractEvent OnInteractionStarted;

public :
	virtual void BeginPlay() override;

public :
	// ��ȣ�ۿ��� �����մϴ�.
	void StartInteraction();

private :
	UFUNCTION()
	void OnPlayerCharacterDetected(
		UPrimitiveComponent*	OverlappedComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComp,
		int32					OtherBodyIndex,
		bool					bFromSweep,
		const FHitResult&		SweepResult);

	UFUNCTION()
	void OnPlayerCharacterLost(
		UPrimitiveComponent*	OverlappedComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComp,
		int32					OtherBodyIndex);

};
