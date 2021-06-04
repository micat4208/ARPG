// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"

IObjectPoolable* UObjectPool::GetRecycledObject() const
{
	// ����� ������ 0 �̶�� nullptr ����
	if (PoolObjects.Num() == 0) return nullptr;

	// �߰��� ��ü�� ���� ������ ��ü�� ã���ϴ�.
	for (auto poolObject : PoolObjects)
	{
		if (poolObject->GetCanRecyclable())
		{
			poolObject->SetCanRecyclable(false);
			poolObject->OnRecycleStart();

			return poolObject;
		}
	}

	return nullptr;
}
