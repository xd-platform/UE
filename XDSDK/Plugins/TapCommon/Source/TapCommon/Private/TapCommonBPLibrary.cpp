// Copyright Epic Games, Inc. All Rights Reserved.

#include "TapCommonBPLibrary.h"
#include "TapCommon.h"
#include "TapBridge.h"
#include "TapJson.h"
#include "TUError.h"
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <ShlObj.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#define TAPCOMMON_REGION_CODE_ID "TAPCOMMON_REGION_CODE_ID"
#define TAPCOMMON_IS_TAPTAP_INSTALLED_ID "TAPCOMMON_IS_TAPTAP_INSTALLED_ID"
#define TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID "TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID"
#define TAPCOMMON_UPDATE_GAME_TAPTAP_ID "TAPCOMMON_UPDATE_GAME_TAPTAP_ID"
#define TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID "TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID"
#define TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID "TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID"
#define TAPCOMMON_OPEN_GAME_TAPTAP_ID "TAPCOMMON_OPEN_GAME_TAPTAP_ID"

UTapCommonBPLibrary::UTapCommonBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    UE_LOG(LogTemp, Warning, TEXT("init TapCommon"));
    Register(TEXT("com.tds.common.wrapper.TDSCommonService"),TEXT("com.tds.common.wrapper.TDSCommonServiceImpl"));
	FTapCommonModule::OnBridgeCallback.AddUObject(this, &UTapCommonBPLibrary::OnBridgeCallback);
#endif
}

void UTapCommonBPLibrary::Register(FString serviceClz, FString serviceImpl)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GetBridge()->Register(serviceClz, serviceImpl);
#endif 
}

void UTapCommonBPLibrary::CallHandler(FString command)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    UE_LOG(LogTemp, Warning, TEXT("CallHandler: %s"), *command);
	GetBridge()->CallHandler(command);
#endif
}

void UTapCommonBPLibrary::GetRegionCode()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"), TEXT("getRegionCode"), TEXT(""), true, TEXT(TAPCOMMON_REGION_CODE_ID),true);
	GetBridge()->CallHandler(commandJson);
#endif
}


void UTapCommonBPLibrary::IsTapTapInstalled(){
#if PLATFORM_ANDROID
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("isTapTapInstalled"),TEXT(""),true,TEXT(TAPCOMMON_IS_TAPTAP_INSTALLED_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapCommonBPLibrary::IsTapGlobalInstalled(){
#if PLATFORM_ANDROID
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("isTapGlobalInstalled"),TEXT(""),true,TEXT(TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapCommonBPLibrary::UpdateGameInTapTap(FString appId){
#if PLATFORM_ANDROID
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("appId"), appId);
    Writer->WriteObjectEnd();
    Writer->Close();
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("updateGameInTapTap"),JsonOutString,true,TEXT(TAPCOMMON_UPDATE_GAME_TAPTAP_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapCommonBPLibrary::UpdateGameInTapGlobal(FString appId){
#if PLATFORM_ANDROID
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("appId"), appId);
    Writer->WriteObjectEnd();
    Writer->Close();
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("updateGameInTapGlobal"),JsonOutString,true,TEXT(TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapCommonBPLibrary::OpenReviewInTapTap(FString appId){
#if PLATFORM_ANDROID
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("appId"), appId);
    Writer->WriteObjectEnd();
    Writer->Close();
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("openReviewInTapTap"),JsonOutString,true,TEXT(TAPCOMMON_OPEN_GAME_TAPTAP_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

void UTapCommonBPLibrary::OpenReviewInTapGlobal(FString appId){
#if PLATFORM_ANDROID
    FString JsonOutString;
    TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonOutString);
    Writer->WriteObjectStart();
    Writer->WriteValue(TEXT("appId"), appId);
    Writer->WriteObjectEnd();
    Writer->Close();
    FString commandJson = TapJson::ConstructorCommand(TEXT("TDSCommonService"),TEXT("openReviewInTapGlobal"),JsonOutString,true,TEXT(TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID),true);
    GetBridge()->CallHandler(commandJson);
#endif
}

TMap<FString, FString> UTapCommonBPLibrary::RemoveEmptyKey(TMap<FString, FString> stringMap)
{
    // FTUError Error;
    // Error.code = 10;
    // Error.error_description = TEXT("哈哈__123'1；231_。。。。");
    // FString ErrorJson;
    // FJsonObjectConverter::UStructToJsonObjectString(FTUError::StaticStruct(), &Error, ErrorJson, 0, 0);
    // UE_LOG(LogTemp, Display, TEXT("hyf__%s"), *ErrorJson);
    stringMap.Remove("");
    return stringMap;
}

TArray<FString> UTapCommonBPLibrary::RemoveEmptyString(TArray<FString> stringArray)
{
    stringArray.Remove("");
    return stringArray;
}

void UTapCommonBPLibrary::OnBridgeCallback(const FString &result)
{
#if PLATFORM_ANDROID || PLATFORM_IOS

    UE_LOG(LogTemp,Warning,TEXT("TapCommon OnBridgeCallback:%s"),*result);
    
    FTapResult tapResult;
    bool parseSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FTapResult>(result,&tapResult,0,0);
    if(!parseSuccess){
        return;
    }

	if(tapResult.callbackId.Equals(TAPCOMMON_REGION_CODE_ID))
	{
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            return;
        }
		FTapCommonRegionCode RegionCodeBean;
		bool JsonParseEnable = FJsonObjectConverter::JsonObjectStringToUStruct<FTapCommonRegionCode>(tapResult.content,&RegionCodeBean,0,0);
        if(!JsonParseEnable)
        {
            return;
        }
		FTapCommonModule::OnRegionCode.Broadcast(RegionCodeBean.isMainland);
        return;
	}

    if(tapResult.callbackId.Equals(TAPCOMMON_IS_TAPTAP_INSTALLED_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnTapTapInstalled.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> taptapInstallReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> taptapInstallRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(taptapInstallReader, taptapInstallRoot);
        if(bSuccessful){
            FTapCommonModule::OnTapTapInstalled.Broadcast(taptapInstallRoot->GetBoolField("isTapTapInstalled"));
            return;
        }
        FTapCommonModule::OnTapTapInstalled.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_IS_TAPGLOBAL_INSTALLED_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnTapGlobalInstalled.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> tapIOInstallReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> tapIOInstallRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(tapIOInstallReader, tapIOInstallRoot);
        if(bSuccessful){
            FTapCommonModule::OnTapGlobalInstalled.Broadcast(tapIOInstallRoot->GetBoolField("isTapGlobalInstalled"));
            return;
        }
        FTapCommonModule::OnTapGlobalInstalled.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_UPDATE_GAME_TAPTAP_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnUpdateGameInTapTap.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> updateTapReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> updateTapRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(updateTapReader, updateTapRoot);
        if(bSuccessful){
            FTapCommonModule::OnUpdateGameInTapTap.Broadcast(updateTapRoot->GetBoolField("updateGameInTapTap"));
            return;
        }
        FTapCommonModule::OnUpdateGameInTapTap.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_UPDATE_GAME_TAPGLOBAL_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnUpdateGameInTapGlobal.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> updateTapIOReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> updateTapIORoot;
        bool bSuccessful = FJsonSerializer::Deserialize(updateTapIOReader, updateTapIORoot);
        if(bSuccessful){
            FTapCommonModule::OnUpdateGameInTapGlobal.Broadcast(updateTapIORoot->GetBoolField("updateGameInTapGlobal"));
            return;
        }
        FTapCommonModule::OnUpdateGameInTapGlobal.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_OPEN_GAME_TAPTAP_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnOpenReviewInTapTap.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> openTapReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> openTapRoot;
        bool bSuccessful = FJsonSerializer::Deserialize(openTapReader, openTapRoot);
        if(bSuccessful){
            FTapCommonModule::OnOpenReviewInTapTap.Broadcast(openTapRoot->GetBoolField("openReviewInTapTap"));
            return;
        }
        FTapCommonModule::OnOpenReviewInTapTap.Broadcast(false);
        return;
    }

    if(tapResult.callbackId.Equals(TAPCOMMON_OPEN_GAME_TAPGLOBAL_ID)){
        if(!UTapCommonBPLibrary::CheckResult(tapResult)){
            FTapCommonModule::OnOpenReviewInTapGlobal.Broadcast(false);
            return;
        }
        TSharedRef<TJsonReader<TCHAR>> openTapIOReader = TJsonReaderFactory<TCHAR>::Create(tapResult.content);
        TSharedPtr<FJsonObject> openTapIORoot;
        bool bSuccessful = FJsonSerializer::Deserialize(openTapIOReader, openTapIORoot);
        if(bSuccessful){
            FTapCommonModule::OnOpenReviewInTapGlobal.Broadcast(openTapIORoot->GetBoolField("openReviewInTapGlobal"));
            return;
        }
        FTapCommonModule::OnOpenReviewInTapGlobal.Broadcast(false);
        return;
    }
#endif 
}

void LaunchWebURL( const FString& URLParams, FString* Error )
{
    UE_LOG(LogTemp, Log, TEXT("LaunchURL %s"), *URLParams);

	FString BrowserOpenCommand;

	// First lookup the program Id for the default browser.
	FString ProgId;
	if (FWindowsPlatformMisc::QueryRegKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\Shell\\Associations\\UrlAssociations\\http\\UserChoice"), TEXT("Progid"), ProgId))
	{
		// If we found it, then lookup it's open shell command in the classes registry.
		FString BrowserRegPath = ProgId + TEXT("\\shell\\open\\command");
		FWindowsPlatformMisc::QueryRegKey(HKEY_CLASSES_ROOT, *BrowserRegPath, NULL, BrowserOpenCommand);
	}

	// If we failed to find a default browser using the newer location, revert to using shell open command for the HTTP file association.
	if (BrowserOpenCommand.IsEmpty())
	{
		FWindowsPlatformMisc::QueryRegKey(HKEY_CLASSES_ROOT, TEXT("http\\shell\\open\\command"), NULL, BrowserOpenCommand);
	}

	// If we have successfully looked up the correct shell command, then we can create a new process using that command
	// we do this instead of shell execute due to security concerns.  By starting the browser directly we avoid most issues.
	if (!BrowserOpenCommand.IsEmpty())
	{
		FString ExePath, ExeArgs;

		// If everything has gone to plan, the shell command should be something like this:
		// "C:\Program Files (x86)\Mozilla Firefox\firefox.exe" -osint -url "%1"
		// We need to extract out the executable portion, and the arguments portion and expand any %1's with the URL,
		// then start the browser process.

		// Extract the exe and any arguments to the executable.
		const int32 FirstQuote = BrowserOpenCommand.Find(TEXT("\""));
		if (FirstQuote != INDEX_NONE)
		{
			const int32 SecondQuote = BrowserOpenCommand.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, FirstQuote + 1);
			if (SecondQuote != INDEX_NONE)
			{
				ExePath = BrowserOpenCommand.Mid(FirstQuote + 1, (SecondQuote - 1) - FirstQuote);
				ExeArgs = BrowserOpenCommand.Mid(SecondQuote + 1);
			}
		}

		// If anything failed to parse right, don't continue down this path, just use shell execute.
		if (!ExePath.IsEmpty())
		{
			if (ExeArgs.ReplaceInline(TEXT("%1"), *URLParams) == 0)
			{
				// If we fail to detect the placement token we append the URL to the arguments.
				// This is for robustness, and to fix a known error case when using Internet Explorer 8. 
				ExeArgs.Append(TEXT(" \"") + URLParams + TEXT("\""));
			}
			ExeArgs = ExeArgs.Replace(TEXT("--single-argument"), TEXT(""));
			// Now that we have the shell open command to use, run the shell command in the open process with any and all parameters.
			if (FPlatformProcess::CreateProc(*ExePath, *ExeArgs, true, false, false, NULL, 0, NULL, NULL).IsValid())
			{
				// Success!
				return;
			}
			else
			{
				if (Error)
				{
					*Error = NSLOCTEXT("Core", "UrlFailed", "Failed launching URL").ToString();
				}
			}
		}
	}

	// // If all else fails just do a shell execute and let windows sort it out.  But only do it if it's an
	// // HTTP or HTTPS address.  A malicious address could be problematic if just passed directly to shell execute.
	// if (URLParams.StartsWith(TEXT("http://")) || URLParams.StartsWith(TEXT("https://")))
	// {
	// 	const HINSTANCE Code = ::ShellExecuteW(NULL, TEXT("open"), *URLParams, NULL, NULL, SW_SHOWNORMAL);
	// 	if (Error)
	// 	{
	// 		*Error = ((PTRINT)Code <= 32) ? NSLOCTEXT("Core", "UrlFailed", "Failed launching URL").ToString() : TEXT("");
	// 	}
	// }
}

void UTapCommonBPLibrary::LaunchURL(const TCHAR* URL, const TCHAR* Param, FString* Error)
{
#if WINDOWS_USE_FEATURE_PLATFORMPROCESS_CLASS
    check(URL);

    if (FCoreDelegates::ShouldLaunchUrl.IsBound() && !FCoreDelegates::ShouldLaunchUrl.Execute(URL))
    {
        if (Error)
        {
            *Error = TEXT("LaunchURL cancelled by delegate");
        }
    }
    else
    {
        // Initialize the error to empty string.
        if (Error)
        {
            *Error = TEXT("");
        }

        // Use the default handler if we have a URI scheme name that doesn't look like a Windows path, and is not http: or https:
        FString SchemeName;
        if (FParse::SchemeNameFromURI(URL, SchemeName) && SchemeName.Len() > 1 && SchemeName != TEXT("http") && SchemeName != TEXT("https"))
        {
            FPlatformProcess::LaunchURL(URL, Param, Error);
        }
        else
        {
            FString URLParams = FString::Printf(TEXT("%s %s"), URL, Param ? Param : TEXT("")).TrimEnd();
            LaunchWebURL(URLParams, Error);
        }
    }
#else
    FPlatformProcess::LaunchURL(URL, Param, Error);
#endif
}

bool UTapCommonBPLibrary::CheckResult(const FTapResult result)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    if(result.code !=0){
        return false;
    }
    return result.content.Len();
#endif
    return false;
}
