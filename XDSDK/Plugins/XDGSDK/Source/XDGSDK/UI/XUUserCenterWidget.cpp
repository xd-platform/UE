#include "XUUserCenterWidget.h"

#include "TapCommon.h"
#include "TUDebuger.h"
#include "TUHUD.h"
#include "TUSettings.h"
#include "XUImpl.h"
#include "XUNet.h"
#include "XUUser.h"
#include "XUUserCenterTipWidget.h"
#include "XDUE.h"
#include "XUConfigManager.h"
#include "XUConfirmWidget.h"
#include "XULoginTypeModel.h"
#include "XUThirdAuthHelper.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "HAL/PlatformApplicationMisc.h"

UXUUserCenterWidget::UXUUserCenterWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
}

void UXUUserCenterWidget::ShowWidget() {
	if (UClass* MyWidgetClass = LoadClass<UXUUserCenterWidget>(
		nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXUUserCenter.BPXUUserCenter_C'"))) {
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UXUUserCenterWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->AddToViewport(TUSettings::GetUILevel());
		}
	}
}

void UXUUserCenterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ScrollPanel->OnUserScrolled.AddDynamic(this, &UXUUserCenterWidget::OnUserScroll);
	LogoutButton->OnClicked.AddDynamic(this, &UXUUserCenterWidget::OnLogoutBtnClick);
}


void UXUUserCenterWidget::NativeConstruct() {
	Super::NativeConstruct();

	userMd = FXUUser::GetLocalModel();
	langModel = XULanguageManager::GetCurrentModel();

	StateLabel->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *langModel->tds_current_account_prefix)));

	const float Offset = ScrollPanel->GetScrollOffset();
	OnUserScroll(Offset);
	
	CurrentLoginTitleLabel->SetText(FText::FromString(GetLoginTypeName()));
	StateLabel2->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *langModel->tds_current_account_prefix)));
	
	IDTitleLabel->SetText(FText::FromString(TEXT("ID: ") + userMd->userId));
	
	ErrorButtonLabel->SetText(FText::FromString(langModel->tds_network_error_retry));
	BindInfoTitleLabel->SetText(FText::FromString(langModel->tds_account_bind_info));

	CloseButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterWidget::OnCloseBtnClick);
	CopyButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterWidget::OnCopyBtnClick);
	ErrorButton->OnClicked.AddUniqueDynamic(this, &UXUUserCenterWidget::OnErrorBtnClick);

	ShouldShowErrorButton(false);

	RequestList();
}

void UXUUserCenterWidget::NativeDestruct() {
	Super::NativeDestruct();
	XUThirdAuthHelper::CancelAllPreAuths();
}

void UXUUserCenterWidget::OnUserScroll(float CurrentOffset)
{
	userMd = FXUUser::GetLocalModel();
	langModel = XULanguageManager::GetCurrentModel();
	
	if (CurrentOffset > 10.f && userMd)
	{
		TitleLabel->SetText(FText::FromString(GetLoginTypeName()));
		StateLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		TitleLabel->SetText(FText::FromString(langModel->tds_account_safe_info));
		StateLabel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UXUUserCenterWidget::OnCloseBtnClick() {
	RemoveFromParent();
}

void UXUUserCenterWidget::OnCopyBtnClick() {
	FPlatformApplicationMisc::ClipboardCopy(*userMd->userId);
	FTapCommonModule::TapThrobberShowToast(langModel->tds_copy_success);
}

void UXUUserCenterWidget::OnErrorBtnClick() {
	RequestList();
}

void UXUUserCenterWidget::OnLogoutBtnClick()
{
	if (ConfirmWidget && ConfirmWidget->IsInViewport())
	{
		ConfirmWidget->RemoveFromParent();
	}
	ConfirmWidget = UXUConfirmWidget::Create(NSLOCTEXT("XDSDK", "Temp1", "确认退出登录？"),
		NSLOCTEXT("XDSDK", "Temp2", "是否确认退出当前账号？"),
		NSLOCTEXT("XDSDK", "Temp3", "取消"),
		NSLOCTEXT("XDSDK", "Temp4", "确认"),
		true);
	check(ConfirmWidget);
	ConfirmWidget->OnWhiteButtonClicked.BindUObject(this, &UXUUserCenterWidget::ConfirmLogout);
	ConfirmWidget->OnBlueButtonClicked.BindUObject(this, &UXUUserCenterWidget::CancelLogout);
	ConfirmWidget->AddToViewport(TUSettings::GetUILevel() + 10);
}

FString UXUUserCenterWidget::GetLoginTypeName() {
	FString Result = XULoginTypeModel::GetName(userMd->GetLoginType());
	if (Result == "Guest" || Result.IsEmpty()) {
		Result = langModel->tds_guest;
	}
	FString unitStr = langModel->tds_account_format;
	return unitStr.Replace(TEXT("%s"), *Result);
}

void UXUUserCenterWidget::RequestList() {
	FTapCommonModule::TapThrobberShowWait();
	XUNet::RequestBindList([=](TSharedPtr<FXUBindResponseModel> Model, FXUError Error) {
		FTapCommonModule::TapThrobberDismiss();
		if (Model.IsValid()) {
			ShouldShowErrorButton(false);
			BindModels.Reset();
			auto SupportItemModels = GetSupportItemModels();
			for (auto SupportItemModel : SupportItemModels) {
				for (auto NetMd : Model->data) {
					if (SupportItemModel->LoginType == NetMd.loginType && NetMd.status == (int)FXDGBindState::Bind) {
						SupportItemModel->BindState = FXDGBindState::Bind; //1未绑定
						break;
					}
				}
				BindModels.Add(SupportItemModel);
			}
			ResetListBox();
		}
		else {
			ShouldShowErrorButton(true);
			TUDebuger::WarningLog(Error.msg);
		}
	});
}

void UXUUserCenterWidget::ResetListBox() {
	ListBox->ClearChildren();
	for (int32 i = 0; i < BindModels.Num(); ++i) {
		UXUUserCenterItemWidget* Item = UXUUserCenterItemWidget::GenerateItem();
		Item->SetBindModel(BindModels[i]);
		Item->SetDividingLineVisible(i != BindModels.Num() - 1);
		ListBox->AddChild(Item);
		Item->BindCallBack = [=](UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model) {
			if (Model->BindState == FXDGBindState::Bind) {
				enum UXUUserCenterTipWidget::AlertType AlertType = GetBindCount() <= 1
					                                                   ? UXUUserCenterTipWidget::DeleteThird
					                                                   : UXUUserCenterTipWidget::UnbindThird;
				UXUUserCenterTipWidget::Show(AlertType, Model->LoginType, [=]() {
					UnBind(CurrentWidget, Model);
				}, nullptr);
			}
			else {
				Bind(CurrentWidget, Model);
			}
		};
	}
}

void UXUUserCenterWidget::ShouldShowErrorButton(bool Should) {
	ErrorSwitcher->SetActiveWidgetIndex(Should ? 1 : 0);
	// if (Should) {
	// 	ListBox->SetVisibility(ESlateVisibility::Collapsed);
	// 	ErrorButton->SetVisibility(ESlateVisibility::Visible);
	// }
	// else {
	// 	ListBox->SetVisibility(ESlateVisibility::Visible);
	// 	ErrorSwitcher->SetActiveWidgetIndex(0);
	// 	ErrorButton->SetVisibility(ESlateVisibility::Collapsed);
	// }
}

TArray<TSharedPtr<XUUserCenterItemModel>> UXUUserCenterWidget::GetSupportItemModels() {
	TArray<TSharedPtr<XUUserCenterItemModel>> UserCenterItemModels;
	auto MD = XUConfigManager::CurrentConfig();
	if (!MD.IsValid()) {
		return UserCenterItemModels;
	}

	auto SDKSupportTypes = XULoginTypeModel::GetSDKSupportTypes();
	for (auto SDKSupportType : SDKSupportTypes) {
		TSharedPtr<XUUserCenterItemModel> Model = MakeShareable(new XUUserCenterItemModel);
		Model->LoginType = SDKSupportType.Type;
		Model->LoginTypeName = SDKSupportType.TypeName;
		for (auto BindEntry : MD->BindEntries) {
			if (Model->LoginTypeName.ToLower() == BindEntry.EntryName.ToLower()) {
				Model->CanBind = BindEntry.CanBind;
				Model->CanUnbind = BindEntry.CanUnbind;
				UserCenterItemModels.Add(Model);
				break;
			}
		}
	}
	return UserCenterItemModels;
}

void UXUUserCenterWidget::DeleteAccount(const FString& Tip) {
	FTapCommonModule::TapThrobberShowToast(Tip);
	XDUE::Logout();
	XUImpl::Get()->ResetPrivacy();
	RemoveFromParent();
}

void UXUUserCenterWidget::Bind(UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model) {
	TFunction<void(TSharedPtr<FJsonObject> paras)> BindBlock = [=](TSharedPtr<FJsonObject> Paras) {
		FTapCommonModule::TapThrobberShowWait();
		XUNet::Bind(Paras, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error) {
			FTapCommonModule::TapThrobberDismiss();
			if (ResponseModel.IsValid()) {
				Model->BindState = FXDGBindState::Bind;
				CurrentWidget->SetBindModel(Model);
				FTapCommonModule::TapThrobberShowToast(langModel->tds_bind_success);
			}
			else {
				if (Error.code > 200) {
					FTapCommonModule::TapThrobberShowToast(Error.msg);
				}
				else {
					FTapCommonModule::TapThrobberShowToast(langModel->tds_bind_error);
				}
			}
			XDUE::OnUserStatusChange.Broadcast(XUType::UserBindSuccess, Model->LoginTypeName);
		});
	};

	XUImpl::Get()->GetAuthParam(Model->LoginType, [=](TSharedPtr<FJsonObject> Paras) {
		                             BindBlock(Paras);
	                             }, [=](FXUError error) {
		                             if (error.code == 80081) {
			                             FTapCommonModule::TapThrobberShowToast(langModel->tds_login_cancel);
		                             }
		                             else {
			                             FTapCommonModule::TapThrobberShowToast(error.msg);
		                             }
	                             });
}

void UXUUserCenterWidget::UnBind(UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model) {
	FTapCommonModule::TapThrobberShowWait();
	XUNet::Unbind(Model->LoginType, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error) {
		FTapCommonModule::TapThrobberDismiss();
		if (ResponseModel.IsValid()) {
			if (GetBindCount() <= 1) {
				DeleteAccount(langModel->tds_unbind_delete_success_return_sign);
			}
			else {
				Model->BindState = FXDGBindState::UnBind;
				CurrentWidget->SetBindModel(Model);
				FTapCommonModule::TapThrobberShowToast(langModel->tds_unbind_success);
			}
		}
		else {
			if (Error.code > 200) {
				FTapCommonModule::TapThrobberShowToast(Error.msg);
			}
			else {
				FTapCommonModule::TapThrobberShowToast(langModel->tds_unbind_guest_return);
			}
		}
		XDUE::OnUserStatusChange.Broadcast(XUType::UserUnBindSuccess, Model->LoginTypeName);
		if (ResponseModel.IsValid()) {
			// 解绑的类型和当前登录的类型一样，那么退出当前账号
			if (userMd->GetLoginType() == Model->LoginType) {
				RemoveFromParent();
				XDUE::OnUserStatusChange.Broadcast(XUType::UserLogout, "");
			}
		}
	});

}

void UXUUserCenterWidget::ConfirmLogout()
{
	if (ConfirmWidget)
	{
		ConfirmWidget->RemoveFromParent();
		ConfirmWidget = nullptr;
	}
	RemoveFromParent();
	XDUE::Logout();
}

void UXUUserCenterWidget::CancelLogout()
{
	if (ConfirmWidget)
	{
		ConfirmWidget->RemoveFromParent();
		ConfirmWidget = nullptr;
	}
}

int UXUUserCenterWidget::GetBindCount() {
	int num = 0;
	for (auto FxdgBindModel : BindModels) {
		if (FxdgBindModel->BindState == FXDGBindState::Bind) {
			num++;
		}
	}
	return num;
}
