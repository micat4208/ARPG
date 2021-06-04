#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"

#include "WndTogglerComponent.generated.h"


DECLARE_DELEGATE(FToggleEvent)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UWndTogglerComponent : public UActorComponent
{
	GENERATED_BODY()

private :
	// ��� ������ ������ ������� �� ȣ���� �븮�ڵ��� ��Ÿ���ϴ�.
	TMap<TSubclassOf<class UObject>, FToggleEvent> ToggleEvents;

public :
	template<typename T>
	FORCEINLINE void RegisterToggleEvent(FToggleEvent toggleEvent)
	{ ToggleEvents.Add(T::StaticClass(), toggleEvent); }

	template<typename T>
	FORCEINLINE void ToggleWnd()
	{
		if (!ToggleEvents.Contains(T::StaticClass())) return;
		ToggleEvents[T::StaticClass()].ExecuteIfBound();
	}
};
