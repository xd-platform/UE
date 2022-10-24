
import shutil
import os
import tds_zip
import sys


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
        tds_zip.zipDir(self.dir_name, f"{self.dir_name}.zip")

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
project_path = os.path.dirname(__file__)
project_path = os.path.join(project_path, "XDSDK")

os.chdir(iOS_frameworks_path)

def update_xdsdk():
    # xdsdk
    Framework("XDSDK/XDAccountSDK.framework", "XDGAccount").update()
    Framework("XDSDK/XDPaymentSDK.framework", "XDGPayment").update()
    Framework("XDSDK/XDCommonSDK.framework", "XDGCommon").update()
    Framework("XDSDK/XDThirdLoginSDK.framework", "XDGCommon").update()
    Framework("XDSDK/XDThirdTrackSDK.framework", "XDGCommon").update()

def update_third_sdk():
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

    def getXCFrameworkPath(name):
        return f"{name}.xcframework/ios-arm64_armv7/{name}.framework"

    # Google
    GooglePath = "ThirdSDK/Frameworks/Google"
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseAnalytics")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseCore")), "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseCoreDiagnostics")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("FirebaseInstallations")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("GoogleAppMeasurement")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("GoogleDataTransport")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("GoogleUtilities")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("nanopb")), "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAnalytics", getXCFrameworkPath("PromisesObjC")), "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseAuth", getXCFrameworkPath("FirebaseAuth")), "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseCrashlytics", getXCFrameworkPath("FirebaseCrashlytics")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseMessaging", getXCFrameworkPath("FirebaseInstanceID")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "FirebaseMessaging", getXCFrameworkPath("FirebaseMessaging")),
              "XDGCommon").update()
    Framework(os.path.join(GooglePath, "GoogleSignIn", getXCFrameworkPath("AppAuth")), "XDGCommon").update()
    Framework(os.path.join(GooglePath, "GoogleSignIn", getXCFrameworkPath("GTMAppAuth")), "XDGCommon").update()
    Framework(os.path.join(GooglePath, "GoogleSignIn", getXCFrameworkPath("GTMSessionFetcher")), "XDGCommon").update()
    Framework(os.path.join(GooglePath, "GoogleSignIn", "GoogleSignIn.framework"), "XDGCommon").update()

    # Line
    Framework("ThirdSDK/Frameworks/Line/LineSDK.framework", "XDGCommon").update()

    # Twitter
    Framework(os.path.join("ThirdSDK/Frameworks/Twitter", getXCFrameworkPath("TDSGTwitterLoginKit")),
              "XDGCommon").update()


update_xdsdk()

