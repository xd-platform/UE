import os
import platform
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
archive_dir = os.path.join(project_dir, "Product")

if isMacPackager:
    engine_root = r"/Users/Shared/Epic\ Games/UE_4.26"
    engine_uat = engine_root + r"/Engine/Build/BatchFiles/RunUAT.sh"
    engine_exe = engine_root + r"/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor"
else:
    engine_root = r"C:\Program Files\Epic Games\UE_4.26"
    engine_uat = engine_root + r"\Engine\Build\BatchFiles\RunUAT.bat"
    engine_exe = engine_root + r"\Engine\Binaries\Win64\UE4Editor-Cmd.exe"


def product_app(target_platform: str):
    if target_platform == mac or target_platform == win:
        target_dir = os.path.join(archive_dir, target_platform)
    else:
        target_dir = archive_dir

    porduct_dir = os.path.join(archive_dir, target_platform)
    if os.path.exists(porduct_dir):
        shutil.rmtree(porduct_dir)
    cmd_argv = [engine_uat,
                "BuildCookRun",
                "-nocompileeditor",
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
        export LANG="C"
        sh {" ".join(cmd_argv)}
        """)
    else:
        ret_value = os.system('call ' + " ".join(cmd_argv))

    if ret_value == 0:
        print(f"打包成功")
        if target_platform == mac:
            zip_file_name = f"{project_name}_Mac.zip"
            print(f"{zip_file_name}压缩中.......")
            tds_zip.zipDir(os.path.join(target_dir, "MacNoEditor"), os.path.join(target_dir, zip_file_name))
            print(f"{zip_file_name}压缩成功")
        if target_platform == win:
            zip_file_name = f"{project_name}_Windows.zip"
            print(f"{zip_file_name}压缩中.......")
            tds_zip.zipDir(os.path.join(target_dir, "WindowsNoEditor"), os.path.join(target_dir, zip_file_name))
            print(f"{zip_file_name}压缩成功")
    else:
        print(f"打包失败: {ret_value}")


# product_app(mac)
product_app(iOS)