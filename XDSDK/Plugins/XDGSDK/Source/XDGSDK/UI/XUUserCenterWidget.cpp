#include "XUUserCenterWidget.h"

#include "TUDebuger.h"
#include "TUHUD.h"
#include "TUSettings.h"
#include "XUImpl.h"
#include "XUNet.h"
#include "XUUser.h"
#include "XUUserCenterTipWidget.h"
#include "XDUE.h"
#include "XUConfigManager.h"
#include "XUThirdAuthHelper.h"
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


void UXUUserCenterWidget::NativeConstruct() {
	Super::NativeConstruct();

	userMd = FXUUser::GetLocalModel();
	langModel = XULanguageManager::GetCurrentModel();

	TitleLabel->SetText(FText::FromString(langModel->tds_account_safe_info));
	InfoTitleLabel->SetText(FText::FromString(langModel->tds_account_info));
	FString Content = FString::Printf(TEXT("%s (%s)"), *langModel->tds_current_account_prefix, *GetLoginTypeName());
	CurrentLoginTitleLabel->SetText(FText::FromString(Content));
	Content = "ID: " + userMd->userId;
	IDTitleLabel->SetText(FText::FromString(Content));
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

void UXUUserCenterWidget::OnCloseBtnClick() {
	RemoveFromParent();
}

void UXUUserCenterWidget::OnCopyBtnClick() {
	FPlatformApplicationMisc::ClipboardCopy(*userMd->userId);
	UTUHUD::ShowToast(langModel->tds_copy_success);
}

void UXUUserCenterWidget::OnErrorBtnClick() {
	RequestList();
}

FString UXUUserCenterWidget::GetLoginTypeName() {
	FString result = langModel->tds_guest;
	if (userMd->GetLoginType() == XUType::TapTap) {
		result = "TapTap";
	} else if (userMd->GetLoginType() == XUType::Google) {
		result = "Google";
	}
	else if (userMd->GetLoginType() == XUType::Apple) {
		result = "Apple";
	}
	FString unitStr = langModel->tds_account_format;
	return unitStr.Replace(TEXT("%s"), *result);
}

void UXUUserCenterWidget::RequestList() {
	UTUHUD::ShowWait();
	XUNet::RequestBindList([=](TSharedPtr<FXUBindResponseModel> Model, FXUError Error) {
		UTUHUD::Dismiss();
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
	for (auto FxdgBindModel : BindModels) {
		UXUUserCenterItemWidget* Item = UXUUserCenterItemWidget::GenerateItem();
		Item->SetBindModel(FxdgBindModel);
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
	if (Should) {
		ListBox->SetVisibility(ESlateVisibility::Collapsed);
		EmptyBox2->SetVisibility(ESlateVisibility::Collapsed);
		ErrorButton->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		ListBox->SetVisibility(ESlateVisibility::Visible);
		EmptyBox2->SetVisibility(ESlateVisibility::Visible);
		ErrorButton->SetVisibility(ESlateVisibility::Collapsed);
	}
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
	UTUHUD::ShowToast(Tip);
	XDUE::Logout();
	XUImpl::Get()->ResetPrivacy();
	RemoveFromParent();
}

void UXUUserCenterWidget::Bind(UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model) {
	TFunction<void(TSharedPtr<FJsonObject> paras)> BindBlock = [=](TSharedPtr<FJsonObject> Paras) {
		UTUHUD::ShowWait();
		XUNet::Bind(Paras, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error) {
			UTUHUD::Dismiss();
			if (ResponseModel.IsValid()) {
				Model->BindState = FXDGBindState::Bind;
				CurrentWidget->SetBindModel(Model);
				UTUHUD::ShowToast(langModel->tds_bind_success);
			}
			else {
				if (Error.code > 200) {
					UTUHUD::ShowToast(Error.msg);
				}
				else {
					UTUHUD::ShowToast(langModel->tds_bind_error);
				}
			}
			XDUE::OnUserStatusChange.Broadcast(XUType::UserBindSuccess, Model->LoginTypeName);
		});
	};

	XUImpl::Get()->GetAuthParam(Model->LoginType, [=](TSharedPtr<FJsonObject> Paras) {
		                             BindBlock(Paras);
	                             }, [=](FXUError error) {
		                             if (error.code == 80081) {
			                             UTUHUD::ShowToast(langModel->tds_login_cancel);
		                             }
		                             else {
			                             UTUHUD::ShowToast(error.msg);
		                             }
	                             });
}

void UXUUserCenterWidget::UnBind(UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<XUUserCenterItemModel> Model) {
	UTUHUD::ShowWait();
	XUNet::Unbind(Model->LoginType, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error) {
		UTUHUD::Dismiss();
		if (ResponseModel.IsValid()) {
			if (GetBindCount() <= 1) {
				DeleteAccount(langModel->tds_unbind_delete_success_return_sign);
			}
			else {
				Model->BindState = FXDGBindState::UnBind;
				CurrentWidget->SetBindModel(Model);
				UTUHUD::ShowToast(langModel->tds_unbind_success);
			}
		}
		else {
			if (Error.code > 200) {
				UTUHUD::ShowToast(Error.msg);
			}
			else {
				UTUHUD::ShowToast(langModel->tds_unbind_guest_return);
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

int UXUUserCenterWidget::GetBindCount() {
	int num = 0;
	for (auto FxdgBindModel : BindModels) {
		if (FxdgBindModel->BindState == FXDGBindState::Bind) {
			num++;
		}
	}
	return num;
}
