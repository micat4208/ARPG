#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDragDropOperation.generated.h"


// ���� �巡�� ��� �۾� ��ü�� ��Ÿ���� ���� Ŭ����
UCLASS()
class ARPG_API USlotDragDropOperation final : 
	public UDragDropOperation
{
	GENERATED_BODY()

public :
	// �巡�׸� ���۽�Ų ���� ��ü�� ��Ÿ���ϴ�.
	class UBaseSlot* OriginatedDragSlot;
	
};
