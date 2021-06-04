#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UPlayerInteractComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	// ��ȣ�ۿ� ������ ��ü���� ������ �迭
	TArray <class UInteractableAreaComponent*> InteractableComponents;

public:	
	UPlayerInteractComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private :
	// �÷��̾� ĳ���Ϳ� ����� ������� �����մϴ�.
	void SortByDistance();

public :
	// ��ȣ�ۿ� ������ ��ü�� �߰��մϴ�.
	void AddInteractable(class UInteractableAreaComponent* newInteractable);

	// ��ȣ�ۿ� ������ ��ü���� ���ܽ�ŵ�ϴ�.
	void RemoveInteractable(class UInteractableAreaComponent* removeInteractable);

	// ��ȣ�ۿ��� �õ��մϴ�.
	void TryInteraction();

		
};
