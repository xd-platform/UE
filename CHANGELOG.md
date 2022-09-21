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





