// Fill out your copyright notice in the Description page of Project Settings.


#include "XDServiceWidgetPayment.h"

#include "XDGPayment.h"
#include "XDGPaymentBPLibrary.h"
#include "XDUE.h"
#include "Components/EditableTextBox.h"
#include "SubWidgets/EditableTextContainerWidget.h"
#include "XDSDK/SubWidgets/ServiceItemWidget.h"
#include "XDSDK/XDSDK.h"

void UXDServiceWidgetPayment::OnPayWithProductClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString OrderIDStr = ETB_PayWithProduct_OrderID->GetText().ToString();
	const FString ProductIDStr = ETB_PayWithProduct_ProductID->GetText().ToString();
	const FString RoleIDStr = ETB_PayWithProduct_RoleID->GetText().ToString();
	const FString ServerIDStr = ETB_PayWithProduct_ServerID->GetText().ToString();
	const FString ExtStr = ETB_PayWithProduct_Ext->GetText().ToString();
	
	UXDGPaymentBPLibrary::PayWithProduct(OrderIDStr, ProductIDStr, RoleIDStr, ServerIDStr, ExtStr);
#endif
}

void UXDServiceWidgetPayment::OnQueryWithProductIdArrayClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const TArray<FString> Arr = ET_Login_LoginTypes->MakeStringArray();
	UXDGPaymentBPLibrary::QueryWithProductIdArray(Arr);
#endif
}

void UXDServiceWidgetPayment::OnQueryRestoredPurchasesClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGPaymentBPLibrary::QueryRestoredPurchases();
#endif
}

void UXDServiceWidgetPayment::OnCheckRefundStatusClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGPaymentBPLibrary::CheckRefundStatus();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	auto Success = [](XUType::CheckPayType CheckType)
	{
		switch (CheckType)
		{
		case XUType::iOSAndAndroid:
			DEMO_LOG(TEXT("iOSAndAndroid"));
			break;
		case XUType::iOS:
			DEMO_LOG(TEXT("iOS"));
			break;
		case XUType::Android:
			DEMO_LOG(TEXT("Android"));
			break;
		case XUType::None:
			DEMO_LOG(TEXT("None"));
			break;
		}
	};
	auto Failed = [](FXUError Error)
	{
		DEMO_LOG_STRING(Error.msg);
	};
	XDUE::CheckPay(Success, Failed);
#endif
}

void UXDServiceWidgetPayment::OnCheckRefundStatusWithUIClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UXDGPaymentBPLibrary::CheckRefundStatusWithUI();
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	auto Success = [](XUType::CheckPayType CheckType)
	{
		switch (CheckType)
		{
		case XUType::iOSAndAndroid:
			DEMO_LOG(TEXT("iOSAndAndroid"));
			break;
		case XUType::iOS:
			DEMO_LOG(TEXT("iOS"));
			break;
		case XUType::Android:
			DEMO_LOG(TEXT("Android"));
			break;
		case XUType::None:
			DEMO_LOG(TEXT("None"));
			break;
		}
	};
	auto Failed = [](FXUError Error)
	{
		DEMO_LOG_STRING(Error.msg);
	};
	XDUE::CheckPay(Success, Failed);
#endif
}

void UXDServiceWidgetPayment::OnPayWithWebClicked()
{
	const FString OrderIDStr = ETB_PayWithWeb_OrderID->GetText().ToString();
	const FString ProductIDStr = ETB_PayWithWeb_ProductID->GetText().ToString();
	const FString ProductNameStr = ETB_PayWithWeb_ProductName->GetText().ToString();
	const float PayAmount = FCString::Atoi(*ETB_PayWithWeb_PayAmount->GetText().ToString());
	const FString RoleIDStr = ETB_PayWithWeb_RoleID->GetText().ToString();
	const FString ServerIDStr = ETB_PayWithWeb_ServerID->GetText().ToString();
	const FString ExtrasStr = ETB_PayWithWeb_Extras->GetText().ToString();


#if PLATFORM_ANDROID
	UXDGPaymentBPLibrary::PayWithWeb(OrderIDStr, ProductIDStr, ProductNameStr, PayAmount, RoleIDStr, ServerIDStr, ExtrasStr);
#elif PLATFORM_WINDOWS || PLATFORM_MAC
	auto Callback = [](XUType::PayResult Result)
	{
		switch (Result)
		{
		case XUType::PayOK:
			DEMO_LOG(TEXT("Pay OK"));
			break;
		case XUType::PayFail:
			DEMO_LOG(TEXT("Pay Fail"));
			break;
		case XUType::PayCancel:
			DEMO_LOG(TEXT("Pay Cancel"));
			break;
		default:
			break;
		}
	};
	XDUE::OpenWebPay(ServerIDStr, RoleIDStr, ProductIDStr, OrderIDStr, ProductNameStr, PayAmount, ExtrasStr, Callback);
#endif
}

void UXDServiceWidgetPayment::OnPurchaseTokenClicked()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	const FString TransactionIdentifierStr = ETB_PurchaseToken_TransactionIdentifier->GetText().ToString();
	const FString ProductIdentifierStr = ETB_PurchaseToken_ProductIdentifier->GetText().ToString();
	const FString OrderIDStr = ETB_PurchaseToken_OrderID->GetText().ToString();
	const FString RoleIDStr = ETB_PurchaseToken_RoleID->GetText().ToString();
	const FString ServerIDStr = ETB_PurchaseToken_ServerID->GetText().ToString();
	const FString ExtStr = ETB_PurchaseToken_Ext->GetText().ToString();
	
	UXDGPaymentBPLibrary::PurchaseToken(TransactionIdentifierStr, ProductIdentifierStr, OrderIDStr, RoleIDStr, ServerIDStr, ExtStr);
#endif
}

void UXDServiceWidgetPayment::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PayWithProduct->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetPayment::OnPayWithProductClicked);
	QueryWithProductIdArray->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetPayment::OnQueryWithProductIdArrayClicked);
	QueryRestoredPurchases->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetPayment::OnQueryRestoredPurchasesClicked);
	CheckRefundStatus->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetPayment::OnCheckRefundStatusClicked);
	CheckRefundStatusWithUI->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetPayment::OnCheckRefundStatusWithUIClicked);
	PayWithWeb->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetPayment::OnPayWithWebClicked);
	PurchaseToken->GetClickButton()->OnClicked.AddDynamic(this, &UXDServiceWidgetPayment::OnPurchaseTokenClicked);

#if PLATFORM_IOS || PLATFORM_ANDROID
	FXDGPaymentModule::OnXDGSDKPaymentSucceed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKPaymentSucceed);
	FXDGPaymentModule::OnXDGSDKPaymentFailed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKPaymentFailed);
    
	FXDGPaymentModule::OnXDGSDKQueryProductIdsSucceed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKQueryProductIdsSucceed);
	FXDGPaymentModule::OnXDGSDKQueryProductIdsFailed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKQueryProductIdsFailed);

	FXDGPaymentModule::OnXDGSDKQueryRestoredPurchasesSucceed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKQueryRestoredPurchasesSucceed);
	FXDGPaymentModule::OnXDGSDKQueryRestoredPurchasesFailed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKQueryRestoredPurchasesFailed);
	FXDGPaymentModule::OnXDGSDKPayWithWebCompleted.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKPayWithWebCompleted);
	FXDGPaymentModule::OnXDGSDKCheckRefundStatusSucceed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKCheckRefundStatusSucceed);
	FXDGPaymentModule::OnXDGSDKCheckRefundStatusFailed.AddUObject(this, &UXDServiceWidgetPayment::OnXDGSDKCheckRefundStatusFailed);
#endif
}

void UXDServiceWidgetPayment::OnXDGSDKPaymentSucceed(const FString& orderId, const FString& productId,
                                                   const FString& serverId, const FString& roleId)
{
	DEMO_LOG(TEXT("OnXDGSDKPaymentSucceed"));
}

void UXDServiceWidgetPayment::OnXDGSDKPaymentFailed(const int32 code, const FString& msg)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKPaymentFailed: code-%d msg-%s"), code, *msg);
}

void UXDServiceWidgetPayment::OnXDGSDKQueryProductIdsSucceed(const FString& resultJson)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKQueryProductIdsSucceed: %s"), *resultJson);
}

void UXDServiceWidgetPayment::OnXDGSDKQueryProductIdsFailed(const int32 code, const FString& msg)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKQueryProductIdsFailed: code-%d msg-%s"), code, *msg);
}

void UXDServiceWidgetPayment::OnXDGSDKQueryRestoredPurchasesSucceed(const FString& resultJson)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKQueryRestoredPurchasesSucceed: -%s"), *resultJson);
}

void UXDServiceWidgetPayment::OnXDGSDKQueryRestoredPurchasesFailed(const int32 code, const FString& msg)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKQueryRestoredPurchasesFailed: code-%d msg-%s"), code, *msg);
}

void UXDServiceWidgetPayment::OnXDGSDKPayWithWebCompleted(const FString& serverId, const FString& roleId,
                                                        const int32 code, const FString& errorMsg)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKPayWithWebCompleted: serverId-%s roleId-%s code-%d errorMsg-%s"), *serverId, *roleId, code, *errorMsg);
}

void UXDServiceWidgetPayment::OnXDGSDKCheckRefundStatusSucceed(const FString& resultJson)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKCheckRefundStatusSucceed: %s"), *resultJson);
}

void UXDServiceWidgetPayment::OnXDGSDKCheckRefundStatusFailed(const int32 code, const FString& msg)
{
	DEMO_LOG(TEXT("点击了 OnXDGSDKCheckRefundStatusFailed: code-%d msg-%s"), code, *msg);
}
