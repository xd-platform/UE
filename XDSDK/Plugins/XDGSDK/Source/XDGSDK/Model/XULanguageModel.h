#pragma once
#include "CoreMinimal.h"
#include "XULanguageModel.generated.h"

USTRUCT()
struct FXULanguageModel
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString tds_account_safe_info;

	UPROPERTY()
	FString tds_account_info;

	UPROPERTY()
	FString tds_account_bind_info;

	UPROPERTY()
	FString tds_unbind;

	UPROPERTY()
	FString tds_bind;

	UPROPERTY()
	FString tds_guest;

	UPROPERTY()
	FString tds_delete_account_title;

	UPROPERTY()
	FString tds_unbind_account_title;

	UPROPERTY()
	FString tds_delete_account_sure_title;

	UPROPERTY()
	FString tds_delete_account;

	UPROPERTY()
	FString tds_unbind_account;

	UPROPERTY()
	FString tds_unbind_account_button;

	UPROPERTY()
	FString tds_delete_account_sure;

	UPROPERTY()
	FString tds_delete_content;

	UPROPERTY()
	FString tds_unbind_content;

	UPROPERTY()
	FString tds_unbind_delete_content;

	UPROPERTY()
	FString tds_unbind_confirm_Content;

	UPROPERTY()
	FString tds_delete_confirm_content;

	UPROPERTY()
	FString tds_input_error;

	UPROPERTY()
	FString tds_cancel;

	UPROPERTY()
	FString tds_input_error_re;

	UPROPERTY()
	FString tds_confirm;

	UPROPERTY()
	FString tds_delete;

	UPROPERTY()
	FString tds_loading;

	UPROPERTY()
	FString tds_current_account_prefix;

	UPROPERTY()
	FString tds_account_format;

	UPROPERTY()
	FString tds_current_id;

	UPROPERTY()
	FString tds_copy_success;

	UPROPERTY()
	FString tds_cancel_bind;

	UPROPERTY()
	FString tds_unbind_error;

	UPROPERTY()
	FString tds_bind_error_reason_format;

	UPROPERTY()
	FString tds_bind_error;

	UPROPERTY()
	FString tds_bind_success;

	UPROPERTY()
	FString tds_unbind_success;

	UPROPERTY()
	FString tds_unbind_success_return_sign;

	UPROPERTY()
	FString tds_unbind_delete_success_return_sign;

	UPROPERTY()
	FString tds_unbind_guest_return;

	UPROPERTY()
	FString tds_loading_error_retry;

	UPROPERTY()
	FString tds_network_error_retry;

	UPROPERTY()
	FString tds_login_success;

	UPROPERTY()
	FString tds_login_cancel;

	UPROPERTY()
	FString tds_login_failed;

	UPROPERTY()
	FString tds_login_button_title;

	UPROPERTY()
	FString tds_confirm_logout_title;

	UPROPERTY()
	FString tds_confirm_logout_content;

	UPROPERTY()
	FString tds_bound;

	UPROPERTY()
	FString tds_user_center_logout;
	
	UPROPERTY()
	FString tds_logout;

	UPROPERTY()
	FString tds_authorization_cancel;

	UPROPERTY()
	FString tds_authorization_failed;

	UPROPERTY()
	FString tds_network_error_login;

	UPROPERTY()
	FString tds_network_error_safe_retry;

	UPROPERTY()
	FString tds_refund_custom_service_tip;

	UPROPERTY()
	FString tds_refund_contact_custom_service;

	UPROPERTY()
	FString tds_refund_custom_service_tip_tail;

	UPROPERTY()
	FString tds_refund_success;

	UPROPERTY()
	FString tds_refund_fail;

	UPROPERTY()
	FString tds_refund_cancel;

	UPROPERTY()
	FString tds_refund_action;

	UPROPERTY()
	FString tds_refund_login_restrict_title;

	UPROPERTY()
	FString tds_refund_login_restrict_sub_title;

	UPROPERTY()
	FString tds_refund_ios_pay_tip;

	UPROPERTY()
	FString tds_pay_success;

	UPROPERTY()
	FString tds_pay_fail;

	UPROPERTY()
	FString tds_pay_cancel;

	UPROPERTY()
	FString tds_button_confirm;

	UPROPERTY()
	FString tds_promotion_exchange_title;

	UPROPERTY()
	FString tds_promotion_exchange_desc;

	UPROPERTY()
	FString tds_refund_android_pay_tip;

	UPROPERTY()
	FString tds_refund_all_pay_tip;

	UPROPERTY()
	FString tds_pay_net_fail;

	UPROPERTY()
	FString tds_refund_net_fail;

	UPROPERTY()
	FString tds_purchase_processing;

	UPROPERTY()
	FString tds_terms_agreement;

	UPROPERTY()
	FString tds_service_terms_agreement;

	UPROPERTY()
	FString tds_push_agreement;

	UPROPERTY()
	FString tds_confirm_agreement;

	UPROPERTY()
	FString tds_is_adult_agreement;

	UPROPERTY()
	FString tds_california_agreement;

	UPROPERTY()
	FString tds_cancel_claim;

	UPROPERTY()
	FString tds_custom_service;

	UPROPERTY()
	FString tds_product_not_exists;

	UPROPERTY()
	FString tds_ios_restricted_payment_title;

	UPROPERTY()
	FString tds_ios_restricted_payment_description;

	UPROPERTY()
	FString xd_agreement_agree;

	UPROPERTY()
	FString xd_agreement_disagree;

	UPROPERTY()
	FString xd_agreement_disagree_confirm_content;

	UPROPERTY()
	FString xd_agreement_disagree_confirm_exit;

	UPROPERTY()
	FString xd_agreement_disagree_confirm_back;

	UPROPERTY()
	FString xd_agreement_age_tips;

	UPROPERTY()
	FString xd_agreement_load_failed;

	UPROPERTY()
	FString tds_retry;

	UPROPERTY()
	FString tds_net_error;

	UPROPERTY()
	FString tds_load_error;

};

		

