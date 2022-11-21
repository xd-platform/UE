# XDGSDK-UEPC-6.0

## XD PC SDK 所包含的插件
* AntiAddiction：国内实名&&防沉迷库
* TapBootstrap：TapSDK辅助启动的库
* TapCommon：基础库，工具库
* TapDB：TapDB库
* TapLogin：TapTap登录的库
* TapMoment：内嵌动态库
* XDGSDK：心动游戏的库

## 项目依赖库文件

将所需要的插件SDK拷贝到项目Plugins文件夹，然后设置依赖库

* 依赖的系统库：
```c#
PrivateDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities" });
```
* 依赖XD SDK：
```c#
PrivateDependencyModuleNames.AddRange(new string[] { "XDGSDK"});
```
* 依赖其余SDK（TapDB，防沉迷等）：
```c#
PrivateDependencyModuleNames.AddRange(new string[] { "TapDB", "AntiAddiction"});
```

## API介绍

先导入XDGSDK的头文件
```c
#include "XDUE.h"
```

## PC

### 初始化
受政策影响，心动首次初始化时（或者协议更新时），将会弹起协议弹窗，只有用户同意协议后，才能初始化成功，否则将会强制退出游戏。

目前有两种初始化方式
```cpp
    // 配置文件初始化
	static void InitSDK(const FString& GameVersion, TFunction<void(bool Result, const FString& Message)> CallBack);
	// 手动初始化
	static void InitSDK(const XUType::Config& Config, TFunction<void(bool Result, const FString& Message)> CallBack);
```
配置文件的默认路径是`/Plugins/XDGSDK/Content/XDGAssets/XDConfig.json`，如果需要配置不同的配置文件，可以在`XDGAssets/`的同级目录下，放入新建的`json`文件，然后在初始化前调用`XUSettings::UpdateConfigFileName`方法来切换需要的初始化配置文件
```cpp
	if (SelectedItem == TEXT("国内")) {
		XUSettings::UpdateConfigFileName("XDConfig-cn.json");
	} else if (SelectedItem == TEXT("海外")) {
		XUSettings::UpdateConfigFileName("XDConfig.json");
	}
```

初始化代码
```cpp
	XDUE::InitSDK("1.2.3", [](bool Result, FString Message) {
		if (Result) {
			TUDebuger::DisplayShow(Message);
		}
		else {
			TUDebuger::WarningShow(Message);
		}
	});
```

如果是手动初始化，那么只需要配置`XUType::Config`值就行了。

### 登录接口

登录接口的类型：
* Default：自动登录
* Guest：游客登录
* Apple：Apple登录（将会支持）
* Google：Google登录
* TapTap：TapTap登录

登录接口调用：
```cpp
    XDUE::LoginByType(LoginType, [](FXUUser User){
        TUDebuger::DisplayShow(TEXT("登录成功：") + TUJsonHelper::GetJsonString(User));
    }, [](FXUError Error)
    {
        if (Error.code == 40021 && Error.ExtraData.IsValid()) {
            FString Platform = Error.ExtraData->GetStringField("loginType");
            FString Email = Error.ExtraData->GetStringField("email");
            TUDebuger::WarningShow(FString::Printf(TEXT("当前 %s 账号所关联的邮箱 %s 未被验证，请前往 %s 验证邮箱后重新登录游戏"), *Platform, *Email, *Platform));
        } else if (Error.code == 40902 && Error.ExtraData.IsValid()) {
            FString Platform = Error.ExtraData->GetStringField("loginType");
            FString Email = Error.ExtraData->GetStringField("email");
            auto Conflicts = Error.ExtraData->GetArrayField("conflicts");
            FString Content = FString::Printf(TEXT("当前 %s 账号所关联的邮箱 %s 对应的游戏账号已绑定"), *Platform, *Email);
            TArray<FString> Accounts;
            for (auto JsonValue : Conflicts) {
                Accounts.Add(JsonValue->AsObject()->GetStringField("loginType"));
            }
            Content += FString::Join(Accounts, TEXT("、"));
            Content += TEXT("，请使用该邮箱所关联的其他平台游戏账号登录后进入「账号安全中心」手动进行账号绑定、解绑操作。");
            TUDebuger::WarningShow(Content);
        } else {
            TUDebuger::WarningShow(TEXT("登录失败：") + TUJsonHelper::GetJsonString(Error) + "\n" + TUJsonHelper::GetJsonString(Error.ExtraData));
        }
    });
```

需要注意的是，对于已登录的用户，每次进游戏需要使用Default方式来更新用户，如果返回的是失败，那么说明心动的登录状态已经失效，需要用户重新登录，游戏方应该退出到登录界面。

登录的某些错误码，会需要游戏方展示弹窗用来引导用户，具体可以咨询相关开发者。

### 主机自动登录

我们可以把「Steam/PS/Nintendo」等平台上发行的游戏称之为主机游戏，对于这些游戏，用户们会更习惯用该平台账号登录游戏。对于这类游戏，推荐使用这个接口来登录。

```cpp
/**
* 主机（Steam/PS/Nintendo 上发行的游戏）自动登录接口
*
* @param SuccessBlock    该主机的账号注册过心动账号，那么会返回登录成功（第二次登录的时候，如果主机账号没有发生改变，会直接返回登录成功，略过网络请求）
* @param FailBlock       该主机的账号未注册过心动账号，那么会返回失败。这个时候需要调用LoginByType登录
* @param ErrorBlock      网络请求错误或者非主机平台的游戏调用，以及其他错误
*/
static void LoginByConsole(TFunction<void(const FXUUser& User)> SuccessBlock, TFunction<void()> FailBlock, TFunction<void(const FXUError& Error)> ErrorBlock);
```

### 获取用户信息及Token
```cpp
	static TSharedPtr<FXUUser> GetUserInfo();
	static TSharedPtr<FXUTokenModel> GetAccessToken();
```
如果是未登录状态，那么指针为空，可以用来判断用户是否已经登录。
使用前判断下指针是否为空，如果登录状态过期，那么也会得到空指针，请及时重新登录。

### 获取用户地域

可以调用下方API获取用户所在地域：
```cpp
	static TSharedPtr<FXUIpInfoModel> GetIPInfo();
```
因为需要用户同意协议之后，才能去获取用户所在地域，所以首次登录游戏可能不能很及时获取`TSharedPtr<FXUIpInfoModel>`，使用前判断下是否为空，如果为空，可以使用接口再次尝试获取：
```cpp
	static void GetIPInfo(TFunction<void(TSharedPtr<FXUIpInfoModel> IpInfo)> CallBack);
```

### 退出登录
游戏用户在退出账户的时候，应该调用
```cpp
    XDUE::Logout();
```

### 注销账户
注销账户时，可以调用
```cpp
	static void AccountCancellation();
```
如果用户注销成功，那么会发出用户被登出的回调：
```cpp
	static XUSimpleDelegate OnLogout;
```
此时，游戏应该做登出的操作。


### 多语言
XDSDK支持14种语言，TapSDK仅支持7种，如果XDSDK设置的语言种类超出了TapSDK的支持，那么在TapSDK中显示的是英语（目前TapSDK仅涉及登录模块）
```cpp
    XDUE::SetLanguage(LangType);
```

### 打开用户中心
目前提供了绑定和解绑的回调。
```cpp
    XDUE::OpenUserCenter(
        [](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
            if (Error.IsValid()) {
                TUDebuger::DisplayShow( FString::Printf(TEXT("绑定失败, Error: %s"), *Error->msg));
            }
            else {
                TUDebuger::DisplayShow( FString::Printf(TEXT("绑定成功, Type: %d"), Type));
            }
        },
        [](XUType::LoginType Type, TSharedPtr<FXUError> Error) {
            if (Error.IsValid()) {
                TUDebuger::DisplayShow( FString::Printf(TEXT("解绑失败, Error: %s"), *Error->msg));
            }
            else {
                TUDebuger::DisplayShow( FString::Printf(TEXT("解绑成功, Type: %d"), Type));
            }
        }
    );
```

### 检查补款
```cpp
    XDUE::CheckPay([](XUType::CheckPayType CheckType)
    {
        switch (CheckType)
        {
        case XUType::iOSAndAndroid:
            TUDebuger::DisplayShow(TEXT("iOSAndAndroid"));
            break;
        case XUType::iOS:
            TUDebuger::DisplayShow(TEXT("iOS"));
            break;
        case XUType::Android:
            TUDebuger::DisplayShow(TEXT("Android"));
            break;
        case XUType::None:
            TUDebuger::DisplayShow(TEXT("None"));
            break;
        }
    }, [](FXUError Error)
    {
        TUDebuger::DisplayShow(Error.msg);
    });
```

### 打开客服中心

```cpp
    XDUE::OpenCustomerCenter("serverId", "roleId", "roleName");
```

### 打开网页支付
国内是扫码支付，国外是打开外部浏览器支付，回调结果仅供参考，发放道具以服务端为准
```cpp
	/**
	* 国外网页支付，国内扫码支付
	*
	* @param ServerId        服务器ID。所在服务器ID，不能有特殊字符，服务端支付回调会包含该字段，必填
	* @param RoleId          角色ID。支付角色ID，服务端支付回调会包含该字段，必填
	* @param ProductId       商品ID。游戏的商品ID（同ProductSkuCode），必填
	* @param OrderId         订单ID。游戏侧订单号，服务端支付回调会包含该字段，可选。
	* @param ProductName     商品名称。用于内嵌支付内部显示，可选
	* @param PayAmount       商品价格，可选
	* @param Ext			 附加信息。服务端支付回调会包含该字段，可选
	* @param CallBack		 支付结果回调
	*/
	static void OpenWebPay(const FString& ServerId,
	                       const FString& RoleId,
	                       const FString& ProductId,
	                       const FString& OrderId,
	                       const FString& ProductName = "",
	                       float PayAmount = 0,
	                       const FString& Ext = "",
	                       TFunction<void(XUType::PayResult Result)> CallBack = nullptr);
```


### 是否允许推送服务
韩国同意协议的时候有个是否统一推送的勾选项，韩国用户登录完后，可以用这个接口获取是否能推送。
```cpp
    if (XDUE::IsPushServiceEnable()) {
        TUDebuger::DisplayShow("Push Service Enable");
    } else {
        TUDebuger::DisplayShow("Push Service Disable");
    }
```


### TapDB埋点
```cpp
	//TapDB 统计用户
	static void TrackUser(FString userId);

	//TapDB 设置属性
	static void TrackRole(FString serverId, FString roleId, FString roleName, int32 level);

	//TapDB 事件埋点
	static void TrackEvent(FString eventName);

	//TapDB 成就埋点
	static void TrackAchievement();

	//TapDB 完成新手引导埋点
	static void EventCompletedTutorial();

	//TapDB 创建角色埋点
	static void EventCreateRole();
```

### 协议相关
```cpp
// 获取子协议列表
TArray<FXUAgreementBean> AgreementList = XUAgreement::GetAgreementList();

// 展示协议链接
XUAgreement::ShowDetailAgreement(Url);

// FXUAgreementBean 定义
USTRUCT()
struct FXUAgreementBean
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString type;

    UPROPERTY()
    FString url;

    FXUAgreementBean() = default;
    FXUAgreementBean(const FString& Type, const FString& Url);
};
```



