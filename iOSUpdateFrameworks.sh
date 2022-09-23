#!/bin/bash

# 使用方式 脚本 iOS库地址
# /Users/huangyifeng/Work/TapSDK/tapsdk2-ue/iOSUpdateFrameworks.sh /Users/huangyifeng/Downloads/Products

cyanPrint() {
    echo -e "\033[0;36m$1\033[0;m"
}

greenPrint() {
    echo -e "\033[0;32m$1\033[0;m"
}

redPrint() {
    echo -e "\033[0;31m$1\033[0;m"
}

# 需要更新的TapSDK文件路径
#NeedUpdateFrameworks=/Users/huangyifeng/Downloads/IOSProducts
NeedUpdateFrameworks=$1
# 脚本所在路径
ShellPath=$(cd "$(dirname $0)";pwd)
# 插件所在路径
PluginsPath=$ShellPath/Plugins

cd $NeedUpdateFrameworks

# 参数1：原生库的名字
# 参数2：所在插件名字
# 参数3：bundle名字（不带bundle后缀）
UpdateFramework() {
    cyanPrint "----------------------$1----------------------"
    FrameworkPath=$PluginsPath/$2/Source/$2/ios/framework
    TempPath=$FrameworkPath/${1##*/}
    ZipPath=${TempPath}.zip
    rm $ZipPath
    mkdir -p $TempPath
    
    cp -r $NeedUpdateFrameworks/${1}.framework ${TempPath}/ && greenPrint "拷贝${1}.framework成功" || redPrint "拷贝${1}.framework失败"
    
    if [ -z "$3" ]
    then
        :
#        echo "\$var is empty"
    else
            cp -r $NeedUpdateFrameworks/${3}.bundle ${TempPath}/ && greenPrint "拷贝${3}.bundle成功" || redPrint "拷贝${3}.bundle失败"
    fi

    cd $FrameworkPath
    zip -rmq $ZipPath ${1##*/}  && greenPrint "压缩${1}.zip成功" || redPrint "压缩${1}.zip失败"
    cyanPrint "-----------------------------------------------------------"
}

# 更新TapBootstrap
UpdateFramework TapBootstrapSDK TapBootstrap

# 更新LeanCloudObjc
UpdateFramework LeanCloudObjc TapBootstrap

# 更新TapMomentSDK
UpdateFramework TapMomentSDK TapMoment TapMomentResource

# 更新TapCommonSDK
UpdateFramework TapCommonSDK TapCommon
#UpdateFramework TapCommonSDK TapCommon TapCommonResource

# 更新TapDB
UpdateFramework TapDB TapDB

# 更新TapFriendSDK
UpdateFramework TapFriendSDK TapFriends

# 更新TapLoginSDK
UpdateFramework TapLoginSDK TapLogin

# 更新AntiAddictionService
#UpdateFramework Dylib/AntiAddictionService AntiAddiction
UpdateFramework AntiAddictionService AntiAddiction

# 更新AntiAddictionUI
UpdateFramework AntiAddictionUI AntiAddiction AntiAdictionResources



