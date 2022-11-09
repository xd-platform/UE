import os.path

import xdsdk_build
import slack_bot


if __name__ == '__main__':
    # 解析需要的平台
    products = xdsdk_build.parse_need_products("-all")
    message = "XDSDK-UE Check"
    if "CI_RUNNER_TAGS" in os.environ:
        message = message + f" ({os.environ['CI_RUNNER_TAGS']})"
    if "CI_JOB_URL" in os.environ:
        message = message + f"\n<{os.environ['CI_JOB_URL']}|PackageStart>"

    thread_id = slack_bot.sendMessage(message)
    for platform_str in products:
        # 打包
        product = xdsdk_build.product_app(platform_str)
        print(f"{platform_str} product: {product}")
        if product is None:
            message = f"{platform_str}打包失败"
            print(message)
            slack_bot.sendMessage(message, thread_id)
            raise Exception(message)

        message = f"{platform_str}打包通过"
        print(message)
        slack_bot.sendMessage(message, thread_id)


