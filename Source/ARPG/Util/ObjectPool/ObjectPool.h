#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Interface/ObjectPoolable.h"

#include "ObjectPool.generated.h"

UCLASS()
class ARPG_API UObjectPool : public UObject
{
	GENERATED_BODY()

private :
	// Ǯ���� ��ü���� ������ �迭
	TArray<IObjectPoolable*> PoolObjects;

public :
	template<typename T>
	FORCEINLINE T* RegisterRecyclableObject(T* newRecyclableObject)
	{
		PoolObjects.Add(static_cast<IObjectPoolable*>(newRecyclableObject));
		return newRecyclableObject;
	}

	template<typename T>
	FORCEINLINE T* GetRecycledObject() const
	{ return Cast<T>(GetRecycledObject()); }

	// ��Ȱ��� ��ü�� ����ϴ�.
	IObjectPoolable* GetRecycledObject() const;

	// Ư���� ���ǽ��� �̿��Ͽ� ��Ȱ��� ��ü�� ����ϴ�.
	template<typename Predicate>
	FORCEINLINE IObjectPoolable* GetRecycledObjectByPred(Predicate pred, bool checkUsing = true) const
	{
		for (auto poolableObject : PoolObjects)
		{
			if (pred(poolableObject) && (checkUsing ? poolableObject->GetCanRecyclable() : true))
				return poolableObject;
		}

		return nullptr;
	}

	// ID �� ���� ��Ȱ��� ��ü�� ����ϴ�.
	FORCEINLINE IObjectPoolable* GetRecycledObjectByID(int32 id) const
	{
		return GetRecycledObjectByPred([this, id](IObjectPoolable* poolObj) 
			{ return poolObj->GetID() == id; });
	}

};
