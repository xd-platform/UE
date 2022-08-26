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

void UXUUserCenterWidget::ShowWidget(TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> BindCallBack,
                                     TFunction<void(XUType::LoginType Type, TSharedPtr<FXUError>)> UnbindCallBack) {
	if (UClass* MyWidgetClass = LoadClass<UXUUserCenterWidget>(
		nullptr, TEXT("WidgetBlueprint'/XDGSDK/BPXUUserCenter.BPXUUserCenter_C'"))) {
		if (TUSettings::GetGameInstance().IsValid()) {
			auto widget = CreateWidget<UXUUserCenterWidget>(TUSettings::GetGameInstance().Get(), MyWidgetClass);
			widget->AddToViewport(TUSettings::GetUILevel());
			widget->BindCallBack = BindCallBack;
			widget->UnbindCallBack = UnbindCallBack;
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
	// else if (userMd->GetLoginType() == XUType::Apple) {
	// 	result = "Apple";
	// }
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
			auto SupportList = GetSupportTypes();
			for (auto st : SupportList) {
				TSharedPtr<FXUBindModel> md = MakeShareable(new FXUBindModel);
				md->loginType = st.TypeValue;
				md->loginName = st.TypeName;
				md->status = (int)FXDGBindType::UnBind;
				for (auto netMd : Model->data) {
					if (st.TypeValue == netMd.loginType && netMd.status == (int)FXDGBindType::Bind) {
						md->status = netMd.status; //1未绑定
						md->bindDate = netMd.bindDate;
						break;
					}
				}
				BindModels.Add(md);
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
		Item->BindCallBack = [=](UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<FXUBindModel> Model) {
			if (Model->status == FXDGBindType::Bind) {
				enum UXUUserCenterTipWidget::AlertType AlertType = GetBindCount() <= 1
					                                                   ? UXUUserCenterTipWidget::DeleteThird
					                                                   : UXUUserCenterTipWidget::UnbindThird;
				UXUUserCenterTipWidget::Show(AlertType, (XUType::LoginType)Model->loginType, [=]() {
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

TArray<XULoginTypeModel> UXUUserCenterWidget::GetSupportTypes() {
	TArray<XULoginTypeModel> list;
	TArray<XULoginTypeModel> SDKList = XULoginTypeModel::GetSDKSupportTypes();
	auto md = XUConfigManager::CurrentConfig();
	for (auto Model : SDKList) {
		for (auto BindEntry : md->LoginEntries) {
			if (Model.TypeName.ToLower() == BindEntry.ToLower()) {
				list.Add(Model);
				break;
			}
		}
	}
	return list;
}

void UXUUserCenterWidget::DeleteAccount(const FString& Tip) {
	UTUHUD::ShowToast(Tip);
	XDUE::Logout();
	XUImpl::Get()->ResetPrivacy();
	RemoveFromParent();
}

void UXUUserCenterWidget::Bind(UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<FXUBindModel> Model) {
	TFunction<void(TSharedPtr<FJsonObject> paras)> BindBlock = [=](TSharedPtr<FJsonObject> Paras) {
		UTUHUD::ShowWait();
		XUNet::Bind(Paras, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error) {
			UTUHUD::Dismiss();
			if (ResponseModel.IsValid()) {
				Model->status = (int)FXDGBindType::Bind;
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
			if (BindCallBack != nullptr) {
				TSharedPtr<FXUError> TempError = nullptr;
				if (!ResponseModel.IsValid()) {
					TempError = MakeShareable(new FXUError(Error));
				}
				int Type = Paras->GetNumberField("type");
				BindCallBack((XUType::LoginType)Type, TempError);
			}
		});
	};

	XUImpl::Get()->GetAuthParam((XUType::LoginType)Model->loginType, [=](TSharedPtr<FJsonObject> Paras) {
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

void UXUUserCenterWidget::UnBind(UXUUserCenterItemWidget* CurrentWidget, TSharedPtr<FXUBindModel> Model) {
	UTUHUD::ShowWait();
	XUNet::Unbind(Model->loginType, [=](TSharedPtr<FXUResponseModel> ResponseModel, FXUError Error) {
		UTUHUD::Dismiss();
		if (ResponseModel.IsValid()) {
			if (GetBindCount() <= 1) {
				DeleteAccount(langModel->tds_unbind_delete_success_return_sign);
			}
			else {
				Model->status = (int)FXDGBindType::UnBind;
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
		if (UnbindCallBack != nullptr) {
			TSharedPtr<FXUError> TempError = nullptr;
			if (!ResponseModel.IsValid()) {
				TempError = MakeShareable(new FXUError(Error));
			}
			UnbindCallBack((XUType::LoginType)Model->loginType, TempError);
		}
	});

}

int UXUUserCenterWidget::GetBindCount() {
	int num = 0;
	for (auto FxdgBindModel : BindModels) {
		if (FxdgBindModel->status == FXDGBindType::Bind) {
			num++;
		}
	}
	return num;
}
