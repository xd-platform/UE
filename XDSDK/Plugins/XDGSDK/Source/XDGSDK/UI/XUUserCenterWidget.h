#pragma once

#include "CoreMinimal.h"
#include "XULanguageManager.h"
#include "XUUser.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "XUUserCenterItemWidget.h"
#include "XUUserCenterWidget.generated.h"


class UXUConfirmWidget;
class UTapAccountToast;
class UScrollBox;
class UWidgetSwitcher;
UCLASS()
class XDGSDK_API UXUUserCenterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UXUUserCenterWidget(const FObjectInitializer& ObjectInitializer);

	static void ShowWidget();

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnUserScroll(float CurrentOffset);

	UFUNCTION()
	void OnCloseBtnClick();

	UFUNCTION()
	void OnCopyBtnClick();

	UFUNCTION()
	void OnErrorBtnClick();

	UFUNCTION()
	void OnLogoutBtnClick();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StateLabel;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentLoginTitleLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StateLabel2;
	
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
	UButton* ErrorButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorButtonLabel;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ErrorSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton* LogoutButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LogoutLabel;
	
	// TFunction<void(bool result)> Completed;

	TSharedPtr<FXUUser> userMd;
	TSharedPtr<FXULanguageModel> langModel;

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

	void ConfirmLogout();

	void CancelLogout();

	UPROPERTY()
	UXUConfirmWidget* ConfirmWidget;
};



