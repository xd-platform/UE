#pragma once

class XURegionConfig {
public:

	virtual ~XURegionConfig() = default;
	virtual FString BaseHost() = 0;
	virtual FString LoginWebHost() = 0;

	//获取配置
	FString InitSDKUrl();
	
	// login
	FString UserProfileUrl();
	
	//游客
	FString CommonLoginUrl();
	
	// 与leanClound同步
	FString LoginSynUrl();

	// 获取用户绑定信息
	FString BindListUrl();

	// 绑定接口
	FString BindInterfaceUrl();

	// 解绑接口
	FString UnbindInterfaceUrl();

	// 查询补款订单信息
	FString PaybackListUrl();

	// 上报同意协议版本
	FString UploadAgreementUrl();
	

	static TSharedPtr<XURegionConfig>& Get();

private:
	static TSharedPtr<XURegionConfig> Instance;
};

