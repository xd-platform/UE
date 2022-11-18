#pragma once
#include "XUNet.h"
#include "XUUser.h"

typedef TFunction<void(bool Result, const FString& Message)> XUInitCallback;

class XUImpl
{
public:
	DECLARE_MULTICAST_DELEGATE(FSimpleDelegate)

	static FSimpleDelegate OnLoginSuccess;
	static FSimpleDelegate OnLogoutSuccess;
	static FSimpleDelegate OnTokenIsInvalid;
	
	enum InitStateType {
		Uninit,
		Initing,
		Inited
	};

	InitStateType InitState = Uninit;
	
	void InitSDK(XUInitCallback CallBack, TFunction<void(TSharedRef<XUType::Config> Config)> EditConfig);
	
	void InitSDK(TSharedPtr<XUType::Config> Config, XUInitCallback CallBack);
	
	void LoginByType(XUType::LoginType LoginType, TFunction<void(const FXUUser& User)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	void LoginByConsole(TFunction<void(const FXUUser& User)> SuccessBlock, TFunction<void()> FailBlock, TFunction<void(const FXUError& Error)> ErrorBlock);

	void GetAuthParam(XUType::LoginType LoginType, TFunction<void(TSharedPtr<FJsonObject> paras)> resultBlock, TFunction<void(FXUError error)> ErrorBlock);

	void CheckPay(TFunction<void(XUType::CheckPayType CheckType)> SuccessBlock, TFunction<void(const FXUError& Error)> FailBlock);

	FString GetCustomerCenter(const FString& ServerId, const FString& RoleId, const FString& RoleName);

	static void OpenWebPay(const FString& ServerId,
						   const FString& RoleId,
						   const FString& ProductId,
						   const FString& OrderId,
						   const FString& ProductName = "",
						   float PayAmount = 0,
						   const FString& Ext = "",
						   TFunction<void(XUType::PayResult Result)> CallBack = nullptr);
	
	void ResetPrivacy();

	void AccountCancellation();

	void Logout();
	
	static TSharedPtr<XUImpl>& Get();

	void BindByType(XUType::LoginType BindType, TFunction<void(bool Success, const FXUError& Error)> CallBack);


private:
	XUImpl();
	static TSharedPtr<XUImpl> Instance;
	
	void RequestKidToken(bool IsConsole, TSharedPtr<FJsonObject> paras, TFunction<void(TSharedPtr<FXUTokenModel> kidToken)> resultBlock, TFunction<void(FXUError error)> ErrorBlock, const FString& ConsoleID = "");

	void RequestUserInfo(TFunction<void(TSharedPtr<FXUUser> ModelPtr)> CallBack, TFunction<void(FXUError Error)> ErrorBlock, TFunction<void(FXUError Error)> TokenInvalidBlock);

	void AsyncNetworkTdsUser(const FString& userId, TFunction<void(FString SessionToken)> callback, TFunction<void(FXUError error)> ErrorBlock);

	void AsyncLocalTdsUser(const FString& userId, const FString& sessionToken);
	
	void InitFinish(XUInitCallback CallBack);

	void RequestServerConfig();

	void LoginSuccess(TSharedPtr<FXUUser> UserPtr, TFunction<void(const FXUUser& User)> SuccessBlock);
	
};
