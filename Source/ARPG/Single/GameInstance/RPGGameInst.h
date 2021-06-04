#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

#include "Single/ManagerClass/ManagerClass.h"

#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"

#include "RPGGameInst.generated.h"

#ifndef GAME_INST
#define GAME_INST
#define GetGameInst(worldContextObj) (Cast<URPGGameInst>(worldContextObj->GetGameInstance()))
#endif

UCLASS()
class ARPG_API URPGGameInst final : 
	public UGameInstance
{
	GENERATED_BODY()

private :
	// ����� �Ŵ��� Ŭ���� �ν��Ͻ��� �����մϴ�.
	TMap<FString, UManagerClass*> ManagerClasses;

private :
	// ManagerClass �� ����մϴ�.
	void RegisterManagerClass(TSubclassOf<UManagerClass> managerClass);

public :
	virtual void Init() override;
	virtual void Shutdown() override;

public :
	template<typename T>
	bool LoadJson(FString fileName, T& outResult, FString keyName = FString(TEXT("default")))
	{
		// ������ ����� ���� ���
		FString folderPath = FPaths::GameDir() + TEXT("/Content/Resources/Json");

		// ��ΰ� �������� �ʴ´ٸ� �ε� ����
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*folderPath))
			return false;

		FString fullPath = folderPath + FString(TEXT("/")) + fileName + FString(TEXT(".json"));

		// ������ �о�ɴϴ�.
		FString jsonStr;
		if (!FFileHelper::LoadFileToString(jsonStr, *fullPath)) return false;
		/// - ���� ������ ���� ���ߴٸ� �ε� ����

		// Json ������ ���� ��ü ����
		TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonStr);

		// Json ������Ʈ ����
		TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

		// ������ȭ�Ͽ� ���ϴ� �����͸� ����ϴ�.
		bool completed = false;
		completed = FJsonSerializer::Deserialize(reader, jsonObj);

		// ������ȭ�� �����ߴٸ�
		if (completed)
		{
			// outResult �� ���� ���� �����մϴ�.
			FJsonObjectConverter::JsonObjectStringToUStruct(
				jsonObj->GetStringField(keyName), &outResult, 0, 0);
		}

		return completed;
	}

	template<typename T>
	void SaveJson(FString fileName, T& saveData, FString keyName = FString(TEXT("default")))
	{
		// ������ ����� ���� ���
		FString folderPath = FPaths::GameDir() + TEXT("/Content/Resources/Json");
		/// - FPaths : ����, ������ ��� ���� �˻��ϱ� ���� ����� �����ϴ� Ŭ����
		/// - GameDir() : ���� ��θ� ��ȯ�մϴ�.

		// ���� ��ΰ� ���ٸ� ��� ����
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*folderPath))
			FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*folderPath);

		// ������ ������(saveData) �� ���ڿ��� ��ȯ�Ͽ� �����մϴ�.
		FString serializedData;
		FJsonObjectConverter::UStructToJsonObjectString(saveData, serializedData);
		/// - FJsonObjectConverter : Json ��ü�� UStruct ���� ��ȯ�� ���� ����� �����ϴ� Ŭ����
		/// - UStructToJsonObjectString() : UStruct ���� Json ���ڿ��� ��ȯ�մϴ�.

		// Json ���Ͽ� �Է��� ���ڿ� ���� ����
		FString jsonStr;
		auto jsonObj = TJsonWriterFactory<>::Create(&jsonStr);
		/// - TJsonWriter : Json ���� ���⿡ ���� ������ �����ϴ� Ŭ����

		// Json ������Ʈ�� �����մϴ�.
		jsonObj->WriteObjectStart();
		// - Json �� �����ʹ� �߰�ȣ�� ���� �������� �ְ�, Ű : ���� ���� ���еǾ� ����.
		// - �� �� key �� string, �������δ� bool, int, float, string Ÿ������ ����� �� ����.
		// - WriteObjectStart() : �߰�ȣ�� ���� Json ������ ������Ʈ�� ������ ������ �ϸ�,
		//   ������ �߰�ȣ�� �ʿ��ϱ� ������ �ۼ��Ǿ���.
		// - ������ �Է��� �����ٸ� Close() �޼��带 ȣ���Ͽ� Json ������ ���� �˸��� �ȴ�.
		// - Json ������ �����ϱ� ���ؼ� ������ �����͸� ���� string ��ü��, ���� ��θ�
		//   FFileHelper::SaveStringToFile() �޼����� �μ��� �����ϸ� �ȴ�.

		// ������ ����
		jsonObj->WriteValue(keyName, serializedData);

		// Json ������Ʈ �� ����
		jsonObj->WriteObjectEnd();

		// Json ������Ʈ ������ �Է� ��
		jsonObj->Close();

		// ������ ���� ��θ� �����մϴ�.
		folderPath += FString(TEXT("/")) + fileName + FString(TEXT(".json"));

		// ���� ����
		FFileHelper::SaveStringToFile(*jsonStr, *folderPath);
	}

public :

	// ����� ManagerClass �� ����ϴ�.
	template<typename ManagerClassType>
	FORCEINLINE ManagerClassType* GetManagerClass()
	{ return Cast<ManagerClassType>(ManagerClasses[ManagerClassType::StaticClass()->GetName()]); }
	
	template<>
	FORCEINLINE FStreamableManager* GetManagerClass<FStreamableManager>()
	{ return &UAssetManager::GetStreamableManager(); }


};
