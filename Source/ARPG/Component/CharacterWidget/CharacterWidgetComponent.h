#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CharacterWidgetComponent.generated.h"

UCLASS()
class ARPG_API UCharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public :
	// ������ �ʱ�ȭ�մϴ�.
	void InitializeCharacterWidget(
		TSubclassOf<class UCharacterWidget> widgetClass,
		FVector2D drawSize);

	// ������ ���̸� �����մϴ�.
	void SetWidgetHeight(float height);

public :
	// ���� ����� CharacterWidget �� ��ȯ�մϴ�.
	class UCharacterWidget* GetCharacterWidget() const;


	
};
