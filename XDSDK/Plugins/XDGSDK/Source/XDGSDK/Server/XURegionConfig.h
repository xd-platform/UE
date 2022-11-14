#pragma once

class XURegionConfig {
public:

	virtual ~XURegionConfig() = default;
	virtual FString BaseHost() = 0;
	virtual FString LoginWebHost() = 0;
	virtual FString TrackHost() = 0;

	//获取配置
	FString InitSDKUrl();

	//获取配置
	FString AgreementUrl();
	
	// 获取用户信息
	FString UserProfileUrl();
	
	// 获取登录Token
	FString CommonLoginUrl();

	// 主机获取登录Token
	FString ConsoleLoginUrl();
	
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

	// 跟踪日志上报
	FString TrackUrl();

	static TSharedPtr<XURegionConfig>& Get();

private:
	static TSharedPtr<XURegionConfig> Instance;
};

