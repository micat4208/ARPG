#include "InteractableAreaComponent.h"

#include "Actor/Character/PlayerCharacter/PlayerCharacter.h"

#include "Component/PlayerInteract/PlayerInteractComponent.h"

void UInteractableAreaComponent::BeginPlay()
{
	Super::BeginPlay();


	OnComponentBeginOverlap.AddDynamic(this, &UInteractableAreaComponent::OnPlayerCharacterDetected);
	OnComponentEndOverlap.AddDynamic(this, &UInteractableAreaComponent::OnPlayerCharacterLost);

}

void UInteractableAreaComponent::StartInteraction()
{
	if (OnInteractionStarted.IsBound())
		OnInteractionStarted.Broadcast(this);
}

void UInteractableAreaComponent::OnPlayerCharacterDetected(
	UPrimitiveComponent*	OverlappedComponent, 
	AActor*					OtherActor, 
	UPrimitiveComponent*	OtherComp, 
	int32					OtherBodyIndex, 
	bool					bFromSweep, 
	const FHitResult&		SweepResult)
{
	if (OtherActor->ActorHasTag(PLAYER_ACTOR_TAG))
	{
		// �ڽ��� ��ȣ�ۿ� ������ ��ü�� �����մϴ�.
		Cast<APlayerCharacter>(OtherActor)->GetPlayerInteract()->AddInteractable(this);
	}
}

void UInteractableAreaComponent::OnPlayerCharacterLost(
	UPrimitiveComponent*	OverlappedComponent, 
	AActor*					OtherActor, 
	UPrimitiveComponent*	OtherComp, 
	int32					OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(PLAYER_ACTOR_TAG))
	{
		// �ڽ��� ��ȣ�ۿ� ������ ��ü���� �����մϴ�.
		Cast<APlayerCharacter>(OtherActor)->GetPlayerInteract()->RemoveInteractable(this);
	}
}
