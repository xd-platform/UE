#pragma once
#include "JsonObjectConverter.h"
#include "TUJsonHelper.h"
#include "TUCrypto.h"

template <typename StructName>
class TUDataStorage {
public:
	static void SaveJsonObject(const FString& Key, const TSharedPtr<FJsonObject>& Value, bool needSaveLocal = true) {
		GetJsonObject()->SetObjectField(Key, Value);
		if (needSaveLocal) { SaveToFile(); }
	}

	static TSharedPtr<FJsonObject> LoadJsonObject(const FString& Key) {
		const TSharedPtr<FJsonObject> *Value = nullptr;
		if (GetJsonObject()->TryGetObjectField(Key, Value)) {
			return *Value;
		}
		else {
			return nullptr;
		}
	}
	
	static void SaveString(const FString& Key, const FString& Value, bool needSaveLocal = true) {
		GetJsonObject()->SetStringField(Key, Value);
		if (needSaveLocal) { SaveToFile(); }
	}

	static FString LoadString(const FString& Key) {
		FString String;
		if (GetJsonObject()->TryGetStringField(Key, String)) {
			return String;
		}
		else {
			return FString();
		}
	}

	static void SaveBool(const FString& Key, bool Value, bool needSaveLocal = true) {
		GetJsonObject()->SetBoolField(Key, Value);
		if (needSaveLocal) { SaveToFile(); }
	}

	static bool LoadBool(const FString& Key) {
		bool Value;
		if (GetJsonObject()->TryGetBoolField(Key, Value)) {
			return Value;
		}
		else {
			return false;
		}
	}

	static void SaveNumber(const FString& Key, double Value, bool needSaveLocal = true) {
		GetJsonObject()->SetNumberField(Key, Value);
		if (needSaveLocal) { SaveToFile(); }
	}

	static double LoadNumber(const FString& Key) {
		double Value;
		if (GetJsonObject()->TryGetNumberField(Key, Value)) {
			return Value;
		}
		else {
			return 0;
		}
	}

	template <typename StructType>
	static void SaveStruct(const FString& Key, const TSharedPtr<StructType>& Value, bool needSaveLocal = true) {
		TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(*Value.Get());
		GetJsonObject()->SetObjectField(Key, jsonObject);
		if (needSaveLocal) { SaveToFile(); }
	}

	template <typename StructType>
	static void SaveStruct(const FString& Key, const StructType& Value, bool needSaveLocal = true) {
		TSharedPtr<FJsonObject> jsonObject = FJsonObjectConverter::UStructToJsonObject(Value);
		GetJsonObject()->SetObjectField(Key, jsonObject);
		if (needSaveLocal) { SaveToFile(); }
	}

	template <typename StructType>
	static TSharedPtr<StructType> LoadStruct(const FString& Key) {
		const TSharedPtr<FJsonObject>* jsonObject;
		if (!GetJsonObject()->TryGetObjectField(Key, jsonObject)) {
			return nullptr;
		}
		TSharedPtr<StructType> value = MakeShareable(new StructType);
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject->ToSharedRef(), value.Get());
		return value;
	}

	static void Remove(const FString& Key, bool needSaveLocal = true) {
		GetJsonObject()->RemoveField(Key);
		if (needSaveLocal) { SaveToFile(); }
	}

	static void SaveToFile() {
		FString filePath = DataStoragePath();
		FString jsonStr = TUJsonHelper::GetJsonString(JsonObject);
		auto data = TUCrypto::AesEncode(TUCrypto::UTF8Encode(jsonStr), TUCrypto::UTF8Encode(DataStorageKey()));
		FFileHelper::SaveArrayToFile(data, *filePath);
	}

private:
	static TSharedPtr<FJsonObject>& GetJsonObject() {
		if (JsonObject == nullptr) {
			FString filePath = DataStoragePath();
			TArray<uint8> data;
			if (FFileHelper::LoadFileToArray(data, *filePath)) {
				auto JsonStr = TUCrypto::UTF8Encode(TUCrypto::AesDecode(data, TUCrypto::UTF8Encode(DataStorageKey())));
				JsonObject = TUJsonHelper::GetJsonObject(JsonStr);
				// UE_LOG(LogTemp, Display, TEXT("JsonStr: %s"), *JsonStr);
			}
		}
		if (JsonObject == nullptr) {
			JsonObject = MakeShareable(new FJsonObject);
		}
		return JsonObject;
	}

	static TSharedPtr<FJsonObject> JsonObject;

	static FString DataStoragePath() {
		if (StructName::StaticStruct()) {
			return FPaths::SandboxesDir() + TEXT("/DataStorage/") + StructName::StaticStruct()->GetName();
		}
		return FPaths::SandboxesDir() + TEXT("/DataStorage/StorageError");
	};

	static FString DataStorageKey() {
		if (StructName::StaticStruct()) {
			return StructName::StaticStruct()->GetName() + TEXT("Key");
		}
		return TEXT("Key");
	}
};

template <typename StructName>
TSharedPtr<FJsonObject> TUDataStorage<StructName>::JsonObject = nullptr;
