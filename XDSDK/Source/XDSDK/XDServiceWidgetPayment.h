// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XDServiceWidgetPayment.generated.h"

class UCheckBox;
class UEditableTextContainerWidget;
class UEditableTextBox;
class UServiceItemWidget;
/**
 * 
 */
UCLASS(Abstract)
class XDSDK_API UXDServiceWidgetPayment : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION()
	void OnPayWithProductClicked();
	
	UFUNCTION()
	void OnQueryWithProductIdArrayClicked();
	
	UFUNCTION()
	void OnQueryRestoredPurchasesClicked();
	
	UFUNCTION()
	void OnCheckRefundStatusClicked();
	
	UFUNCTION()
	void OnCheckRefundStatusWithUIClicked();
	
	UFUNCTION()
	void OnPayWithWebClicked();
	
	UFUNCTION()
	void OnPurchaseTokenClicked();

protected:
	virtual void NativeOnInitialized() override;

	void OnXDGSDKPaymentSucceed(const FString&, const FString&, const FString&, const FString&);

	void OnXDGSDKPaymentFailed(const int32, const FString&);

	void OnXDGSDKQueryProductIdsSucceed(const FString&);

	void OnXDGSDKQueryProductIdsFailed(const int32, const FString&);

	void OnXDGSDKQueryRestoredPurchasesSucceed(const FString&);

	void OnXDGSDKQueryRestoredPurchasesFailed(const int32, const FString&);

	void OnXDGSDKPayWithWebCompleted(const FString&, const FString&, const int32, const FString&);

	void OnXDGSDKCheckRefundStatusSucceed(const FString&);

	void OnXDGSDKCheckRefundStatusFailed(const int32, const FString&);

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithProduct_OrderID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithProduct_ProductID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithProduct_RoleID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithProduct_ServerID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithProduct_Ext;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* PayWithProduct;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextContainerWidget* ET_Login_LoginTypes;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* QueryWithProductIdArray;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* QueryRestoredPurchases;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* CheckRefundStatus;
	
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* CheckRefundStatusWithUI;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithWeb_OrderID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithWeb_ProductID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithWeb_ProductName;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithWeb_PayAmount;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithWeb_RoleID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithWeb_ServerID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PayWithWeb_Extras;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* PayWithWeb;

	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PurchaseToken_TransactionIdentifier;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PurchaseToken_ProductIdentifier;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PurchaseToken_OrderID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PurchaseToken_RoleID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PurchaseToken_ServerID;
	UPROPERTY(Meta = (BindWidget))
	UEditableTextBox* ETB_PurchaseToken_Ext;
	UPROPERTY(Meta = (BindWidget))
	UServiceItemWidget* PurchaseToken;
};
