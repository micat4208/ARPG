#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseSlot/ItemSlot/ItemSlot.h"
#include "Struct/ItemSlotInfo/ItemSlotInfo.h"

#include "Enum/PartsType.h"
#include "PlayerEquipSlot.generated.h"

UCLASS()
class ARPG_API UPlayerEquipSlot : public UItemSlot
{
	GENERATED_BODY()

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PartsName;

private :
	// ������ ǥ���ϴ� ������ ��Ÿ���ϴ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPartsType PartsType;

protected :
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

public :
	void UpdateEquipSlot(FItemSlotInfo& itemSlotInfo);

	// ��� ������ ���� ������ŵ�ϴ�.
	/// - inventorySlotIndex : �������̴� ��� �������� ��� �κ��丮 ���Կ� �߰��� �������� �����մϴ�.
	void DetachToEquipItemSlot(int32 inventorySlotIndex = INDEX_NONE);

public :
	FORCEINLINE EPartsType GetPartsType() const
	{ return PartsType; }




};
