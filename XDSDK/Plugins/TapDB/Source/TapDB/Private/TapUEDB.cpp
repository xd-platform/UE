#include "TapUEDB.h"

#include "TUDBImpl.h"
#include "TUDebuger.h"

static bool IsInitialized = false;

void TapUEDB::Init(TUDBType::Config Config) {
	if (IsInitialized) {
		TUDebuger::WarningShow("Has Initialized");
		return;
	}
	IsInitialized = true;
	TUDBImpl::Get()->Init(Config);
}

void TapUEDB::SetUserWithLoginType(const FString& UserId, const FString& LoginType) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->SetUserWithLoginType(UserId, LoginType);
}

void TapUEDB::ClearUser() {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->ClearUser();
}

void TapUEDB::SetName(const FString& Name) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->SetName(Name);
}

void TapUEDB::SetLevel(int Level) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->SetLevel(Level);
}

void TapUEDB::SetServer(const FString& Server) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->SetServer(Server);
}

void TapUEDB::OnCharge(const FString& OrderId, const FString& Product, int Amount, const FString& CurrencyType,
	const FString& Payment, TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->OnCharge(OrderId, Product, Amount, CurrencyType, Payment, Properties);
}

void TapUEDB::TrackEvent(const FString& EventName, TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->TrackEvent(EventName, Properties);
}

void TapUEDB::RegisterStaticProperties(TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->RegisterStaticProperties(Properties);
}

void TapUEDB::UnregisterStaticProperty(const FString& Key) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->UnregisterStaticProperty(Key);
}

void TapUEDB::ClearStaticProperties() {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->ClearStaticProperties();
}

void TapUEDB::RegisterDynamicProperties(TFunction<TSharedPtr<FJsonObject>()> PropertiesBlock) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->RegisterDynamicProperties(PropertiesBlock);
}

void TapUEDB::DeviceInitialize(TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->DeviceInitialize(Properties);
}

void TapUEDB::DeviceUpdate(TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->DeviceUpdate(Properties);
}

void TapUEDB::DeviceAdd(TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->DeviceAdd(Properties);
}

void TapUEDB::UserInitialize(TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->UserInitialize(Properties);
}

void TapUEDB::UserUpdate(TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->UserUpdate(Properties);
}

void TapUEDB::UserAdd(TSharedPtr<FJsonObject> Properties) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->UserAdd(Properties);
}

void TapUEDB::AdvertiserIDCollectionEnabled(bool Enable) {
	if (!IsInitialized) {
		TUDebuger::WarningShow(FString::Printf(TEXT("Please Init First Before Call %s"), ANSI_TO_TCHAR(__FUNCTION__)));
		return;
	}
	TUDBImpl::Get()->AdvertiserIDCollectionEnabled(Enable);
}
