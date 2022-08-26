#pragma once

class TAPCOMMON_API TUDebuger
{
public:

	
	static bool IsTest;
	static TMap<FString, FString> ReplaceHosts;
	static TMap<FString, FString> ReplaceOtherContents; // 除了host以外，也需要在test模式下替换的内容，比如说key之类的。
	
	static void DisplayLog(const FString& Info);
	static void WarningLog(const FString& Info);
	static void ErrorLog(const FString& Info);

	static void DisplayShow(const FString& Info, float Duration = 3.f);
	static void WarningShow(const FString& Info, float Duration = 3.f);
	static void ErrorShow(const FString& Info, float Duration = 3.f);

};
