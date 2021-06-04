#include "CharacterWidgetComponent.h"

#include "Widget/CharacterWidget/CharacterWidget.h"

void UCharacterWidgetComponent::InitializeCharacterWidget(
	TSubclassOf<UCharacterWidget> widgetClass,
	FVector2D drawSize)
{
	// ���� Ŭ������ �����մϴ�.
	SetWidgetClass(widgetClass);

	// ũ�⸦ �����մϴ�.
	SetDrawSize(drawSize);

	// ������ ǥ�õ� ������ �����մϴ�.
	SetWidgetSpace(EWidgetSpace::Screen);
}

void UCharacterWidgetComponent::SetWidgetHeight(float height)
{
	SetRelativeLocation(FVector::UpVector * height);
}

UCharacterWidget* UCharacterWidgetComponent::GetCharacterWidget() const
{
	return Cast<UCharacterWidget>(GetUserWidgetObject());
}
