import os.path

import xdsdk_build
import sys
import upload_npkg
import slack_bot


if __name__ == '__main__':
    # 解析需要的平台
    products = xdsdk_build.parse_need_products(" ".join(sys.argv[1:]))
    if len(products) == 0:
        exit(0)
    message = "XDSDK-UE Build"
    if "CI_RUNNER_TAGS" in os.environ:
        message = message + f" ({os.environ['CI_RUNNER_TAGS']})"
    if "CI_JOB_URL" in os.environ:
        message = message + f"\n<{os.environ['CI_JOB_URL']}|PackageStart>"

    thread_id = slack_bot.sendMessage(message)
    for platform_str in products:
        # 打包
        product = xdsdk_build.product_app(platform_str)
        if product is None:
            message = f"{platform_str}打包失败"
            print(message)
            slack_bot.sendMessage(message, thread_id)
            continue
        # 上传NPKG
        url = upload_npkg.Uploader(product).upload()
        if len(url) < 0:
            message = f"{product}上传失败"
            print(message)
            slack_bot.sendMessage(message, thread_id)
            continue
        message = f"{platform_str}下载地址：{url}"
        print(message)
        slack_bot.sendMessage(message, thread_id)
        # 移动模块上传NPKG
        # if platform_str == xdsdk_build.iOS or platform_str == xdsdk_build.android:
        #     url = upload_npkg.Uploader(product).upload()
        #     if len(url) < 0:
        #         message = f"{product}上传失败"
        #         print(message)
        #         slack_bot.sendMessage(message, thread_id)
        #         continue
        #     message = f"{platform_str}下载地址：{url}"
        #     print(message)
        #     slack_bot.sendMessage(message, thread_id)
        # else:
        #     upload_success = slack_bot.uploadFile(product, thread_id)
        #     if upload_success is not True:
        #         message = f"{product}上传失败"
        #         print(message)
        #         slack_bot.sendMessage(message, thread_id)


