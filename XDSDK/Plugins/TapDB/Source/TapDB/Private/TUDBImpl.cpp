#include "TUDBImpl.h"

#include "TUDebuger.h"
#if PLATFORM_MAC || PLATFORM_WINDOWS
#include "Desktop/TUDBPCImpl.h"
#elif PLATFORM_IOS || PLATFORM_ANDROID
#include "Mobile//TUDBMobileImpl.h"
#endif

TSharedPtr<TUDBImpl> TUDBImpl::Instance = nullptr;

TSharedPtr<TUDBImpl>& TUDBImpl::Get() {
	if (!Instance.IsValid()) {
#if PLATFORM_MAC || PLATFORM_WINDOWS
		Instance = MakeShareable(new TUDBPCImpl);
#elif PLATFORM_IOS || PLATFORM_ANDROID
		Instance = MakeShareable(new TUDBMobileImpl);
#else
		Instance = MakeShareable(new TUDBImpl);
#endif
	}
	return Instance;
}

void TUDBImpl::Init(TUDBType::Config InitConfig) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::SetUserWithLoginType(const FString& UserId, const FString& LoginType) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::ClearUser() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::SetName(const FString& Name) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::SetLevel(int Level) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::SetServer(const FString& Server) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
	const FString& Payment, TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::UnregisterStaticProperty(const FString& Key) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::ClearStaticProperties() {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::UserAdd(TSharedPtr<FJsonObject> Properties) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}

void TUDBImpl::AdvertiserIDCollectionEnabled(bool Enable) {
	TUDebuger::ErrorLog("Unsupported Platforms");
}
