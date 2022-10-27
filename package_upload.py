import os.path

import xdsdk_build
import sys
import upload_npkg
import slack_bot

if __name__ == '__main__':
    # 解析需要的平台
    products = xdsdk_build.parse_need_products(" ".join(sys.argv[1:]))
    products = (xdsdk_build.iOS, xdsdk_build.mac)
    thread_id = slack_bot.sendMessage("XDSDK-UE Build")
    for platform_str in products:
        # 打包
        product = xdsdk_build.product_app(platform_str)
        if len(product) < 0:
            message = f"{platform_str}打包失败"
            print(message)
            slack_bot.sendMessage(message, thread_id)
            continue
        # 移动模块上传NPKG
        if platform_str == xdsdk_build.iOS or platform_str == xdsdk_build.android:
            url = upload_npkg.Uploader(product).upload()
            if len(url) < 0:
                message = f"{product}上传失败"
                print(message)
                slack_bot.sendMessage(message, thread_id)
                continue
            message = f"{platform_str}下载地址：{url}"
            print(message)
            slack_bot.sendMessage(message, thread_id)
        else:
            upload_success = slack_bot.uploadFile(product, thread_id)
            if upload_success is not True:
                message = f"{product}上传失败"
                print(message)
                slack_bot.sendMessage(message, thread_id)


