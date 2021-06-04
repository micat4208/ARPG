#pragma once

#include "CoreMinimal.h"
#include "Widget/ClosableWnd/ClosableWnd.h"
#include "Enum/PartsType.h"
#include "PlayerEquipItemWnd.generated.h"

UCLASS()
class ARPG_API UPlayerEquipItemWnd : public UClosableWnd
{
	GENERATED_BODY()

private :
	// â�� ǥ�õǴ� ���Ե��� ��Ÿ���ϴ�.
	TMap<EPartsType, class UPlayerEquipSlot*> EquipSlots;

protected :
	virtual void NativeConstruct() override;

public :
	// ���� ���Ե��� �����մϴ�.
	void UpdatePartsSlot();

public :
	FORCEINLINE class UPlayerEquipSlot* GetPlayerEquipSlot(EPartsType partsType)
	{
		if (!EquipSlots.Contains(partsType)) return nullptr;
		return EquipSlots[partsType];
	}

	
};
