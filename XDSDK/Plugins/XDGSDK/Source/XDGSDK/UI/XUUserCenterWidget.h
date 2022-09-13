#pragma once

#include "CoreMinimal.h"
#include "XULanguageManager.h"
#include "XUBindModel.h"
#include "XUError.h"
#include "XULoginTypeModel.h"
#include "XUUser.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "XUUserCenterItemWidget.h"
#include "XUUserCenterWidget.generated.h"


UCLASS()
class XDGSDK_API UXUUserCenterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUUserCenterWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowWidget(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack, TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack);

protected:
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnCloseBtnClick();

	UFUNCTION()
	void OnCopyBtnClick();

	UFUNCTION()
	void OnErrorBtnClick();

private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InfoTitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentLoginTitleLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* IDTitleLabel;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BindInfoTitleLabel;

	UPROPERTY(meta = (BindWidget))
	UButton* CopyButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ListBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* EmptyBox2;

	UPROPERTY(meta = (BindWidget))
	UButton* ErrorButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorButtonLabel;
	
	
	
	// TFunction<void(bool result)> Completed;

	TSharedPtr<FXUUser> userMd;
	TSharedPtr<FXULanguageModel> langModel;

	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack;
	TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack;

	FString GetLoginTypeName();

	void RequestList();

	void ResetListBox();

	void ShouldShowErrorButton(bool Should);

	TArray<TSharedPtr<XUUserCenterItemModel>> GetSupportItemModels();

	TArray<TSharedPtr<XUUserCenterItemModel>> BindModels;

	void DeleteAccount(const FString& Tip);

	int GetBindCount();

	void Bind(UXUUserCenterItemWidget *CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model);

	void UnBind(UXUUserCenterItemWidget *CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model);

	

	
};

