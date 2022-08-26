# TapBootstrap 启动器

TapBootstrap 用于 TapSDK 中的内建账户登陆、用户信息获取等功能

```c++
#include "TapUEBootstrap.h"
```

## 接口描述

### 初始化

```cpp
	TUType::Config Config;
	Config.ClientID = ClientID;
	Config.ClientToken = ClientToken;
	Config.ServerURL = ServerURL;
	Config.RegionType = (TUType::RegionType)RegionType;
	TapUEBootstrap::Init(Config);
```

### TDS 内建账户登陆

在你使用 TDS 内建登陆时调用以下接口:

```c++
/**
 * 调用 TDS 内建账户登陆 (TapTap 登录)
 * @param permissions TUType::PermissionScope
 */
	TapUEBootstrap::Login({TUType::PermissionScope::Profile}, [](const FTapUser& User) {
		TUDebuger::DisplayShow(TUJsonHelper::GetJsonString(User));
	}, [](const FTUError& Error) {
		TUDebuger::WarningShow(TUJsonHelper::GetJsonString(Error));
	});

/**
 * 游客(匿名)登录
 */
	TapUEBootstrap::AnonymouslyLogin([](const FTapUser& User) {
		TUDebuger::DisplayShow(TUJsonHelper::GetJsonString(User));
	}, [](const FTUError& Error) {
		TUDebuger::WarningShow(TUJsonHelper::GetJsonString(Error));
	});

```


### 退出登录

```c++
	TapUEBootstrap::Logout();
```

### 获取 TDS 用户信息

```c++
	TSharedPtr<FTapUser> UserPtr = TapUEBootstrap::GetUser();
	if (UserPtr.IsValid()) {
		TUDebuger::DisplayShow(TUJsonHelper::GetJsonString(UserPtr));
	} else {
		TUDebuger::WarningShow("No User");
	}
```

### 设置语言

```c++
	TapUEBootstrap::SetPreferLanguage((TUType::LanguageType)LangType);
```