
import shutil
import os
import zipfile
import sys


def zipDir(source_dir, output_filename):
    """
    压缩指定文件夹
    :param source_dir: 目标文件夹路径
    :param output_filename: 压缩文件保存路径+xxxx.zip
    :return: 无
    """
    if os.path.exists(source_dir):
        # compression压缩比例，默认是不压缩，ZIP_LZMA=14级别的压缩，影响的是时间，但是包能更小
        # vue使用级别=9的，var archive = archiver('zip', {zlib: {level: 9}});
        # mac install: brew install xz
        # mode 解压是 r , 压缩是 w 追加压缩是 a
        # compression 为  zipfile.ZIP_DEFLATED，zipfile.ZIP_STORED， zipfile.ZIP_LZMA
        zipf = zipfile.ZipFile(file=output_filename, mode='w', compression=zipfile.ZIP_DEFLATED)
        # zipf = zipfile.ZipFile(file=output_filename, mode='w', compression=zipfile.ZIP_DEFLATED)
        pre_len = len(os.path.dirname(source_dir))
        for parent, dirnames, filenames in os.walk(source_dir):
            for filename in filenames:
                pathfile = os.path.join(parent, filename)
                arcname = pathfile[pre_len:].strip(os.path.sep)  # 相对路径
                zipf.write(pathfile, arcname)
        zipf.close()
        return output_filename
    return


def unzip_file(zip_src, dst_dir):
    '''
    解压缩
    '''
    r = zipfile.is_zipfile(zip_src)
    if r:
        fz = zipfile.ZipFile(zip_src, 'r')
        for file in fz.namelist():
            fz.extract(file, dst_dir)
    else:
        print('This is not zip')
        return False
    return True

# iOS_frameworks_path = "/Users/sisiliu/Downloads/Products"
# project_path = "/Users/sisiliu/Documents/hyfwork/tapsdk2-ue"

class Framework(object):
    def __init__(self, source_path, target_plugin, bundle_path=""):
        self.source_path = source_path
        self.bundle_path = bundle_path
        self.name = os.path.basename(source_path)
        self.dir_name = os.path.splitext(self.name)[0]
        self.bundle_name = os.path.basename(bundle_path)
        self.target_plugin = target_plugin

    def update(self):
        if not os.path.exists(self.source_path):
            print(f"没有找到{self.source_path}")
            return

        self.generate_content_dir()
        self.zip_content_dir()
        self.copy_zip()

        shutil.rmtree(self.dir_name)
        os.remove(f"{self.dir_name}.zip")
        print(f"update {self.name} success")

    def generate_content_dir(self):
        if os.path.exists(self.dir_name):
            shutil.rmtree(self.dir_name)
        # os.makedirs(self.name) 帮我们做了
        shutil.copytree(self.source_path, os.path.join(self.dir_name, self.name))
        if len(self.bundle_name) > 0:
            shutil.copytree(self.bundle_path, os.path.join(self.dir_name, self.bundle_name))
            # shutil.copy(src_file, target_path) 拷贝文件，上面是拷贝文件夹

    def zip_content_dir(self):
        zipDir(self.dir_name, f"{self.dir_name}.zip")

    def copy_zip(self):
        dest_path = os.path.join(project_path,
                                 "Plugins",
                                 self.target_plugin,
                                 "Source",
                                 "ThirdParty",
                                 "iOS",
                                 "Frameworks")
        if not os.path.exists(dest_path):
            os.makedirs(dest_path)
        shutil.copy(f"{self.dir_name}.zip", dest_path)


iOS_frameworks_path = sys.argv[1]
project_path = os.path.join(os.getcwd(), "XDSDK");

os.chdir(iOS_frameworks_path)

# xdsdk
Framework("XDSDK/XDAccountSDK.framework", "XDGAccount").update()
Framework("XDSDK/XDPaymentSDK.framework", "XDGPayment").update()
Framework("XDSDK/XDCommonSDK.framework", "XDGCommon").update()
Framework("XDSDK/XDThirdLoginSDK.framework", "XDGCommon").update()
Framework("XDSDK/XDThirdTrackSDK.framework", "XDGCommon").update()
# Framework("TapMomentSDK.framework", "TapMoment", "TapMomentResource.bundle").update()

# Adjust
Framework("ThirdSDK/Frameworks/Adjust/AdjustSdk.framework", "XDGCommon").update()
# AppsFlyer
Framework("ThirdSDK/Frameworks/AppsFlyer/AppsFlyerLib.framework", "XDGCommon").update()
# Facebook
FacebookPath = "ThirdSDK/Frameworks/Facebook"
Framework(os.path.join(FacebookPath, "FBAEMKit.framework"), "XDGCommon").update()
Framework(os.path.join(FacebookPath, "FBSDKCoreKit_Basics.framework"), "XDGCommon").update()
Framework(os.path.join(FacebookPath, "FBSDKCoreKit.framework"), "XDGCommon").update()
Framework(os.path.join(FacebookPath, "FBSDKLoginKit.framework"), "XDGCommon").update()
Framework(os.path.join(FacebookPath, "FBSDKShareKit.framework"), "XDGCommon").update()

def getXCFrameworkPath(name) :
    return f"{name}.xcframework/ios-arm64_armv7/{name}.framework"
# Google
GooglePath = "ThirdSDK/Frameworks/Google"
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseAnalytics")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseCore")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseCoreDiagnostics")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseInstallations")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("GoogleAppMeasurement")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("GoogleDataTransport")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("GoogleUtilities")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("nanopb")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("PromisesObjC")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseAuth", getXCFrameworkPath("FirebaseAuth")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseCrashlytics", getXCFrameworkPath("FirebaseCrashlytics")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseMessaging", getXCFrameworkPath("FirebaseInstanceID")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "FirebaseMessaging", getXCFrameworkPath("FirebaseMessaging")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "GoogleSignIn", getXCFrameworkPath("AppAuth")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "GoogleSignIn", getXCFrameworkPath("GTMAppAuth")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "GoogleSignIn", getXCFrameworkPath("GTMSessionFetcher")), "XDGCommon").update()
Framework(os.path.join(GooglePath, "GoogleSignIn", "GoogleSignIn.framework"), "XDGCommon").update()

# Line
Framework("ThirdSDK/Frameworks/Line/LineSDK.framework", "XDGCommon").update()

# Twitter
Framework(os.path.join("ThirdSDK/Frameworks/Twitter", getXCFrameworkPath("TDSGTwitterLoginKit")), "XDGCommon").update()

