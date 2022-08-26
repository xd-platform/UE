# AntiAddiction

目前只有国内PC端

## 初始化

```c++
	AAUType::Config Config;
	Config.GameID = ClientID;
	Config.ShowSwitchAccount = true;
	AntiAddictionUE::Init(Config);
```
`ShowSwitchAccount`如果为True，那么未成年在无法进入游戏或者被可玩时长限制而踢出游戏时，显示的弹窗上有有切换账户的选项，点击后，会触发`AntiAddictionUE::OnSwitchAccount`的回调。

`AntiAddictionUE::OnExit`会在未成年触发时限强制关闭游戏时调用，游戏方在这个回调中，用来保存用户数据。

## 启动防沉迷
```c++
	AntiAddictionUE::StartUp(UserIDTF->Text.ToString(), [](AAUType::StartUpResult Result) {
		if (Result == AAUType::StartUpSuccess) {
			TUDebuger::DisplayShow(TEXT("防沉迷登录成功"));
		}
		if (Result == AAUType::TimeLimit) {
			TUDebuger::DisplayShow(TEXT("未成年不可登录"));
		}
		if (Result == AAUType::StartUpCancel) {
			TUDebuger::DisplayShow(TEXT("登录取消"));
		}
		if (Result == AAUType::RealNameVerifying) {
			TUDebuger::DisplayShow(TEXT("身份正在审核中"));
		}
	});
```

## 进入&&退出游戏

相关API：
```c++
	AntiAddictionUE::EnterGame();
	AntiAddictionUE::LeaveGame();
```
在游戏进入后台，或者从后台激活时，可以调用相关API，`LeaveGame`会暂停防沉迷的计时，`EnterGame`会开始防沉迷的计时

## 防沉迷登出
```c++
	AntiAddictionUE::Logout();
```

在游戏退出的时候，防沉迷一起退出