import os
import platform
import sys

import tds_zip
import shutil

isMacPackager = platform.system() == "Darwin"

# Win32,Win64,HoloLens,Mac,XboxOne,PS4,IOS,Android,HTML5,Linux,LinuxAArch64,AllDesktop,TVOS,Switch,Lumin
android = "Android"
iOS = "IOS"
mac = "Mac"
win = "Win64"
# 'Debug', 'DebugGame', 'Development', 'Shipping', 'Test'
client_config = "Development"

# 需要设置的值
project_dir = os.path.join(os.path.dirname(__file__), "XDSDK")
project_name = "XDSDK"
archive_dir = os.path.join(os.path.dirname(__file__), "Product")
iOS_product_name = "Distro_XDSDK.ipa"
android_product_name = "XDSDK-armv7.apk"

if isMacPackager:
    engine_root = r"/Users/Shared/Epic\ Games/UE_4.26"
    # engine_root = r"~/SourceCode/UnrealEngine"

    engine_uat = engine_root + r"/Engine/Build/BatchFiles/RunUAT.sh"
    engine_exe = engine_root + r"/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor"
else:
    engine_root = r"C:\Program Files\Epic Games\UE_4.26"
    engine_uat = engine_root + r"\Engine\Build\BatchFiles\RunUAT.bat"
    engine_uat = f'"{engine_uat}"'
    engine_exe = engine_root + r"\Engine\Binaries\Win64\UE4Editor-Cmd.exe"
    engine_exe = f'"{engine_exe}"'


def parse_need_products(argvs_str: str):
    parse_str = argvs_str.lower()
    results = set()
    if "-ios" in parse_str:
        results.add(iOS)
    if "-android" in parse_str:
        results.add(android)
    if "-win" in parse_str:
        results.add(win)
    if "-mac" in parse_str:
        results.add(mac)
    if "-mobile" in parse_str:
        results.add(iOS)
        results.add(android)
    if "-pc" in parse_str:
        results.add(mac)
        results.add(win)
    if "-all" in parse_str:
        results.add(iOS)
        results.add(android)
        results.add(mac)
        results.add(win)

    if isMacPackager:
        results = results.difference({win})
        if "AndroidPackageDisableOnMac" in os.environ:
            results = results.difference({android})
    else:
        results = results.difference({mac, iOS})
        if "AndroidPackageDisableOnWindow" in os.environ:
            results = results.difference({android})
    print(f"package: {results}")
    return tuple(results)


def product_app(target_platform: str):
    # mobile 平台会自己生成 该平台的文件夹，pc的不会.target_dir只用于打包
    if target_platform == mac or target_platform == win:
        target_dir = os.path.join(archive_dir, target_platform)
    else:
        target_dir = archive_dir
    # porduct_dir是产物的路径
    porduct_dir = os.path.join(archive_dir, target_platform)
    if os.path.exists(porduct_dir):
        shutil.rmtree(porduct_dir)
    cmd_argv = [engine_uat,
                "BuildCookRun",
                "-installed",
                "-nop4",
                f"-project={os.path.join(project_dir, f'{project_name}.uproject')}",
                "-cook",
                "-stage",
                "-archive",
                f"-archivedirectory={target_dir}",
                "-package",
                f"-ue4exe={engine_exe}",
                "-clean",
                "-ddc=InstalledDerivedDataBackendGraph",
                "-pak",
                "-prereqs",
                "-distribution",
                "-nodebuginfo",
                f"-targetplatform={target_platform}",
                "-build",
                f"-target={project_name}",
                f"-clientconfig={client_config}",
                "-utf8output"]

    if isMacPackager:
        ret_value = os.system(f"""
        export LANG=C.UTF-8
        sh {" ".join(cmd_argv)}
        """)
    else:
        ret_value = os.system('call ' + " ".join(cmd_argv))

    if ret_value == 0:
        print(f"打包成功")
        final_product = ""
        if target_platform == mac:
            zip_file_name = f"{project_name}_Mac.zip"
            tds_zip.zipDir(os.path.join(porduct_dir, "MacNoEditor"), os.path.join(porduct_dir, zip_file_name))
            final_product = os.path.join(porduct_dir, zip_file_name)
        elif target_platform == win:
            zip_file_name = f"{project_name}_Windows.zip"
            tds_zip.zipDir(os.path.join(porduct_dir, "WindowsNoEditor"), os.path.join(porduct_dir, zip_file_name))
            final_product = os.path.join(porduct_dir, zip_file_name)
        elif target_platform == iOS:
            final_product = os.path.join(porduct_dir, iOS_product_name)
        elif target_platform == android:
            final_product = os.path.join(porduct_dir, android_product_name)

        if len(final_product) > 0:
            return final_product
    else:
        print(f"打包失败: {ret_value}")


if __name__ == '__main__':
    products = parse_need_products(" ".join(sys.argv[1:]))
    for platform_str in products:
        print(product_app(platform_str))
    # print(product_app(mac))
    # product_app(iOS)
    # product_app(android)
