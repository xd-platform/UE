# 6.8.0 （2022年11月22日）

## PC SDK
### New Feature
- 支持 Steam 登录
- 添加主机登录接口


## Mobile SDK
### New Feature
- 支持 Steam 登录
- 增加 logout 埋点
- 创建订单接口增加参数：
    - price: 当前货币价格（带有货币符号）
    - priceAmountMicros: 当前货币的价格，单位微分

# 6.7.0 （2022年11月8日）

## PC SDK
### New Feature
- 协议逻辑调整：用户主动退出登录时会触发协议新签弹窗
- 新增获取当前子协议列表和展示协议链接窗口的 API
- 新增 TrackUser 不需要参数的接口，会默认传递当前 xdid


## Mobile SDK
### New Feature
- [Android][iOS] 协议逻辑调整：用户主动退出登录时会触发协议新签弹窗
- [Android][iOS] 新增获取当前子协议列表和展示协议链接窗口的 API
- [Android][iOS] 新增 TrackUser 不需要参数的接口，会默认传递当前 xdid
- [Android][iOS] 以 Twitter 登录的用户如果 Twitter token 失效会拦截自动登录
- [iOS] iOS 13 以下的系统支持以网页授权的方式登录 Apple 账号
- [iOS] 新增 token 失效的 Code (4101)，自动登录失效的 msg 为 "XD token expired."


# 6.6.0 （2022年10月18日）

## Mobile SDK
### New Feature
- RegionType 为 Global 的网页支付内嵌 WebView 已废弃，改为使用 Chrome Custom Tabs 实现。
- 支持 TapTap 登录使用 basic_info 的权限，在沙盒中可使用无感登录

### Optimization
* 协议弹窗、个人中心弹窗按钮增加按压状态

### Fixed bugs
- 修复在某些情况下，在内嵌动态中登录 Tap 账户时，XDUser 被切换的问题

# 6.5.3
## PC SDK
### New Feature
* 海外支付新增回调
* 支付添加埋点

### Optimization
* 部分语言UI适配
* 内嵌Web页加载loading优化

### Breaking Changes
* PC配置json文件中`webpay_url`改换为`webpay_url_for_pc`
* 支付回调的枚举值`PaySuccess`改换为`PayOK`

### Fixed bugs
* 解绑当前登录的账号类型时，未被踢出登录状态的bug

## Mobile SDK
### New Feature
* Android & iOS 的阿里云日志支持上传 AppsFlyer 的 UID

### Optimization
* 协议弹窗、个人中心弹窗按钮增加按压状态

### Fixed bugs
* iOS 修复调用 TrackEvent 时会崩溃的问题
* iOS 初始化配置时如果服务端下发 region 会更新（之前本地缓存读出来之后就不更新了）
* AppsFlyer 的 ATT 等待时长从 600s 缩短到 120s 以避免过长的等待时间会丢失过多的设备新装

# 6.5.2
## PC SDK
### New Feature
* 新增苹果登录
* 新增对TapDB的封装Track API

### Optimization
* 协议适配高分辨率屏幕尺寸
* 用户中心删除字段`Delete`匹配大小写敏感
* 其他部分UI细节优化

### Fixed bugs
* 修复国内协议弹出失败的bug
* 修复了打开外部浏览器会被游戏页覆盖的bug
* 修复用户中心只展示TapTap绑定栏的bug
* 修复用户中心绑定选项服务不可配置的bug





