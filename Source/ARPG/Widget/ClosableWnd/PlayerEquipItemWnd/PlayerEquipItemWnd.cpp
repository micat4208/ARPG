#include "PlayerEquipItemWnd.h"

#include "Single/GameInstance/RPGGameInst.h"
#include "Single/PlayerManager/PlayerManager.h"

#include "Widget/BaseSlot/ItemSlot/PlayerEquipSlot/PlayerEquipSlot.h"

void UPlayerEquipItemWnd::NativeConstruct()
{
	Super::NativeConstruct();

	// â ũ�� ����
	WndSize = FVector2D(500.0f, 800.0f);

	// â�� ������ ���� ���� ã��
	{
		// ã�� ���� ������ �̸��� ��Ÿ���ϴ�.
		TArray<FName> equipSlotNames = {
			FName(TEXT("BP_PlayerEquipSlot_Top")),
			FName(TEXT("BP_PlayerEquipSlot_Glove")),
			FName(TEXT("BP_PlayerEquipSlot_Bottom")),
			FName(TEXT("BP_PlayerEquipSlot_Shoes")),
			FName(TEXT("BP_PlayerEquipSlot_Weapon"))
		};

		// ���� ���� �̸����� �̿��Ͽ� ���� �������� ã���ϴ�.
		for (FName slotWidgetName : equipSlotNames)
		{
			UPlayerEquipSlot* equipSlot = Cast<UPlayerEquipSlot>(GetWidgetFromName(slotWidgetName));
			EquipSlots.Add(equipSlot->GetPartsType(), equipSlot);
		}
	}

	// ��� ���Ե��� �����մϴ�.
	UpdatePartsSlot();
}

void UPlayerEquipItemWnd::UpdatePartsSlot()
{
	// ���� �������� �����۵��� �����ϴ� �����̳ʸ� ����ϴ�.
	TMap<EPartsType, FItemSlotInfo>& currentPartsInfo = GetManager(UPlayerManager)->GetPlayerInfo()->PartsInfos;

	// ��� ���Ե��� �����մϴ�.
	for (auto equipSlot : EquipSlots)
	{
		// ���� �������� ��� �������� equipSlot �� ���� ������ ���� ������ ����ϴ�.
		FItemSlotInfo& partsSlotInfo = currentPartsInfo[equipSlot.Key];

		equipSlot.Value->UpdateEquipSlot(partsSlotInfo);

	}

}
