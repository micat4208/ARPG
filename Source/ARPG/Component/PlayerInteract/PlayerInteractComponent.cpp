#include "PlayerInteractComponent.h"

#include "Component/InteractableArea/InteractableAreaComponent.h"

UPlayerInteractComponent::UPlayerInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInteractComponent::SortByDistance()
{
	if (InteractableComponents.Num() < 2) return;

	InteractableComponents.Sort(
		[this](const UInteractableAreaComponent& A, const UInteractableAreaComponent& B)
		{
			// �÷��̾� ĳ������ ��ġ�� �����մϴ�.
			FVector ownerLocation = GetOwner()->GetActorLocation();

			FVector aLocation = A.GetOwner()->GetActorLocation();
			FVector bLocation = B.GetOwner()->GetActorLocation();

			float aDist = FVector::DistXY(ownerLocation, aLocation);
			float bDist = FVector::DistXY(ownerLocation, bLocation);

			return aDist > bDist;
		});

}

void UPlayerInteractComponent::AddInteractable(UInteractableAreaComponent* newInteractable)
{
	if (!InteractableComponents.Contains(newInteractable))
	{
		InteractableComponents.Add(newInteractable);
		SortByDistance();
	}
	/// TArrayInst.Contains(item) : TArrayInst �� item �� ��ġ�ϴ� ���� ���� ��Ұ� 
	/// �����ϴ��� Ȯ���մϴ�.
	/// �����Ѵٸ� true
}

void UPlayerInteractComponent::RemoveInteractable(UInteractableAreaComponent* removeInteractable)
{
	if (InteractableComponents.Contains(removeInteractable))
	{
		InteractableComponents.Remove(removeInteractable);
		SortByDistance();
	}
	/// - TArrayInst.Remove(item) : TArrayInst ���� item �� ��ġ�ϴ� ���� ���� ��Ҹ� �����մϴ�.

}

void UPlayerInteractComponent::TryInteraction()
{
	// ��ȣ�ۿ� ������ ��ü�� �������� �ʴٸ� ���� X
	if (InteractableComponents.Num() == 0) return;

	InteractableComponents[0]->StartInteraction();


}

