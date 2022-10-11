package com.xd;

import com.xd.intl.common.XDGSDK;
import android.net.Uri;
import com.xd.intl.common.component.share.XDGShareType;
import android.content.res.Configuration;
import android.util.DisplayMetrics;
import android.util.Log;
import android.app.Activity;
import android.view.Display;
import android.view.View;
import android.widget.Toast;
import android.os.Handler;
import android.os.Looper;

import android.Manifest;
import android.app.Activity;
import android.graphics.Bitmap;
import android.text.TextUtils;
import android.util.Log;

import com.xd.intl.common.callback.Callback;
import com.tds.common.bridge.Bridge;
import com.tds.common.bridge.utils.BridgeJsonHelper;
import com.tds.common.permission.RequestPermissionCallback;
import com.tds.common.permission.TdsPermission;
import com.tds.common.widgets.image.ImageTarget;
import com.tds.common.widgets.image.TdsImage;
// import com.xd.intl.common.component.share.XDGShareCallback;
import com.xd.intl.common.XDGSDK;
import com.xd.intl.common.callback.XDGInitCallback;
import com.xd.intl.common.bean.XDGRegionInfo;
import com.xd.intl.common.utils.XDConfigManager;
import com.xd.intl.common.widget.TDSToastManager;
import com.xd.intl.common.tracker.aliyun.LoginLogger;
import com.xd.intl.common.entities.TapDBConfig;
import com.xd.intl.common.entities.TapSdkConfig;
import com.xd.intl.common.base.XDGError;
import com.xd.intl.common.utils.CurrentSessionDataManager;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import com.xd.intl.common.utils.EnvHelper;
import com.xd.intl.common.utils.FileUtils;

public class XDGCommonUnreal4 {

    private static int width = -1;
    private static int height = -1;
    public static int orientation = -1;
    public static Activity tmpActivity;

    public static void initSDK(final Activity activity) {
        print("点击初始化");

        if (!XDGSDK.isInitialized()) {
            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override
                public void run() {
                  
                    XDGSDK.init(activity, new XDGInitCallback() {
                        @Override
                        public void initCallback(boolean success, String message) {
                            Map<String, Object> resultParams = new HashMap<>();
                            resultParams.put("success", success);
            
                            Map<String, Object> configInfoParams = new HashMap<>();
                            TapSdkConfig tapSdkConfig = XDConfigManager.getInstance().getXdConfig().tapSdkConfig;
                            configInfoParams.put("region", XDConfigManager.getInstance().isRegionTypeCN() ? "0" : "1");
                            if (tapSdkConfig != null) {
                                configInfoParams.put("clientId", tapSdkConfig.clientId);
                                configInfoParams.put("clientToken", tapSdkConfig.clientToken);
                                configInfoParams.put("serverURL", tapSdkConfig.serverUrl);
            
                                Map<String, Object> dbConfigParam = new HashMap<>();
                                TapDBConfig tapDBConfig = tapSdkConfig.tapDBConfig;
                                if (tapDBConfig != null) {
                                    dbConfigParam.put("enableTapDB", tapDBConfig.enable);
                                    dbConfigParam.put("channel", tapDBConfig.channel);
                                    dbConfigParam.put("gameVersion", tapDBConfig.gameVersion);
                                }
                                configInfoParams.put("dbConfig", dbConfigParam);
                            }
                            resultParams.put("configInfo", configInfoParams);
            
                            if (!success) {
                                resultParams.put("message", message);
                            }

                            String result = BridgeJsonHelper.object2JsonString(resultParams);
                            nativeOnXDGSDKInitCompleted(success, result);
                        }
                    });
                }
            });
        }
    }

    public static boolean isInitialized() {
        boolean isInit = XDGSDK.isInitialized();
        print("点击是否初始化: " + isInit);
        return isInit;
    }

    public static void setLanguage(int languageType) {
        print("点击设置语言:" + languageType);
        XDGSDK.setLanguage(languageType);
    }

    public static void report(String serverId, String roleId, String roleName) {
        print("点击 report:" + serverId + " roleId:" + roleId + " roleName:" + roleName);
        XDGSDK.report(serverId, roleId, roleName);
    }

    public static void trackUser(String userId) {
        print("点击trackUser");
        XDGSDK.trackUser(userId);
    }

    public static void trackRole(String serverId,
            String roleId,
            String roleName,
            int level) {
        print("点击 trackUser:" + serverId + " roleId:" + roleId + " roleName:" + roleName + " level:" + level);
        XDGSDK.trackRole(serverId, roleId, roleName, level);
    }

    public static void trackEvent(String eventName) {
        print("点击trackEvent" + eventName);
        XDGSDK.trackEvent(eventName);
    }


    public static String getSDKVersionName() {
        String v = XDGSDK.getVersionName();
        print("点击 getSDKVersionName: " + v);
        return v;
    }

    public static void shareFlavors(int shareFlavors, String uri,  String message) {
        // print("点击 share:" + shareFlavors + " uri:" + uri + " message:" + message);

        // // 要在主线程执行
        // new Handler(Looper.getMainLooper()).post(new Runnable() {
        //     @Override
        //     public void run() {

        //         @XDGShareType.Type
        //         int type = XDGShareType.FACEBOOK;
        //         if (shareFlavors == 1) {
        //             type = XDGShareType.LINE;
        //         } else if (shareFlavors == 2) {
        //             type = XDGShareType.TWITTER;
        //         }

        //         XDGSDK.share(type, uri, message, new XDGShareCallback() {
        //             @Override
        //             public void shareSuccess() {
        //                 nativeOnXDGSDKShareCompleted(0);
        //             }

        //             @Override
        //             public void shareCancel() {
        //                 nativeOnXDGSDKShareCompleted(1);
        //             }

        //             @Override
        //             public void shareFailed(String error) {
        //                 nativeOnXDGSDKShareCompleted(2);
        //             }
        //         });
        //     }
        // });
    }

    public static void shareImage(int shareFlavors, String imagePath) { // 安卓uri path string
        // print("点击 share 2个参数 shareFlavors：" + shareFlavors + "  imagePath: " + imagePath);

        // // 要在主线程执行
        // new Handler(Looper.getMainLooper()).post(new Runnable() {
        //     @Override
        //     public void run() {

        //         @XDGShareType.Type
        //         int type = XDGShareType.FACEBOOK;
        //         if (shareFlavors == 1) {
        //             type = XDGShareType.LINE;
        //         } else if (shareFlavors == 2) {
        //             type = XDGShareType.TWITTER;
        //         }

        //         if (type == XDGShareType.FACEBOOK) {
        //             try {
        //                 File file = new File(imagePath);
        //                 if (file.exists()) {
        //                     Bitmap bitmap = BitmapFactory.decodeFile(imagePath);
        //                     XDGSDK.share(type, bitmap, new XDGShareCallback() {
        //                         @Override
        //                         public void shareSuccess() {
        //                             nativeOnXDGSDKShareCompleted(0);
        //                         }

        //                         @Override
        //                         public void shareCancel() {
        //                             nativeOnXDGSDKShareCompleted(1);
        //                         }

        //                         @Override
        //                         public void shareFailed(String error) {
        //                             nativeOnXDGSDKShareCompleted(2);
        //                         }
        //                     });
        //                 }
        //             } catch (Exception e) {
        //                 e.printStackTrace();
        //                 nativeOnXDGSDKShareCompleted(2);
        //             }

        //         } else {
        //             Uri uri = Uri.parse(new File(imagePath).toString());
        //             XDGSDK.share(type, uri, new XDGShareCallback() {
        //                 @Override
        //                 public void shareSuccess() {
        //                     nativeOnXDGSDKShareCompleted(0);
        //                 }

        //                 @Override
        //                 public void shareCancel() {
        //                     nativeOnXDGSDKShareCompleted(1);
        //                 }

        //                 @Override
        //                 public void shareFailed(String error) {
        //                     nativeOnXDGSDKShareCompleted(2);
        //                 }
        //             });
        //         }
        //     }
        // });
    }

    public static void storeReview() {
        print("点击 storeReview");
        XDGSDK.storeReview();
    }

    public static void setCurrentUserPushServiceEnable(boolean enable) {
        print("点击 setCurrentUserPushServiceEnable " + enable);
        XDGSDK.setCurrentUserPushServiceEnable(enable);
    }

    public static boolean isCurrentUserPushServiceEnable() {
        boolean r = XDGSDK.isCurrentUserPushServiceEnable();
        print("点击 isCurrentUserPushServiceEnable" + r);
        return r;
    }

    public static void getRegionInfo() {
        XDGSDK.getRegionInfo(new Callback<XDGRegionInfo>() {
            @Override
            public void onCallback(XDGRegionInfo info, XDGError xdgError) {
                if (info != null){
                    nativeOnXDGSDKGetRegionInfoCompleted(info.countryCode, info.city, info.timeZone, info.locationInfoType);
                }else{
                    nativeOnXDGSDKGetRegionInfoCompleted("", "", "", "");
                }
            }
        });       
    }


    public static void getXDGInfoJson(final Activity gameActivity){
        String json = FileUtils.getJsonFromAssetsFile(gameActivity, "XDConfig.json");
        nativeOnXDGSDKGetXDGInfoJsonCompleted(json);
    }


    public static void disableAgreementUI() {
        EnvHelper.setAgreementUIEnable(false);
    }

    public static void setCountryRegion(String region){
        EnvHelper.setTargetCountryOrRegion(region);
    }


    public static void developInit(final Activity activity, int num) {
            // 0: 海外 正式	1：海外 RND	2：国内 正式	3.国内 RND
            if (num == 0) {
                EnvHelper.updateConfigFileName("io_config_pro.json");
            } else if (num == 1) {
                EnvHelper.updateConfigFileName("io_config_test.json");
            } else if (num == 2) {
                EnvHelper.updateConfigFileName("cn_config_pro.json");
            } else if (num == 3) {
                EnvHelper.updateConfigFileName("cn_config_test.json");
            }
        
            if (num == 0 || num == 2) {
                EnvHelper.setApiEnv(EnvHelper.EnvEnum.Dev);
            } else if (num == 1 || num == 3) {
                EnvHelper.setApiEnv(EnvHelper.EnvEnum.Product);
            }
            initSDK(activity);
    }

    public static void trackAchievement() {
        print("点击 trackAchievement");
        XDGSDK.trackAchievement();
    }

    public static void eventCompletedTutorial() {
        print("点击 eventCompletedTutorial");
        XDGSDK.eventCompletedTutorial();
    }

    public static void eventCreateRole() {
        print("点击 eventCreateRole");
        XDGSDK.eventCreateRole();
    }


    // 下面是与SDK无关方法
    public static void onCreate(final Activity activity) {
        Display display = activity.getWindowManager().getDefaultDisplay();
        DisplayMetrics dm = new DisplayMetrics();
        display.getRealMetrics(dm);

        if (activity.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
            width = dm.widthPixels;
            height = dm.heightPixels;
        }
    }

    public static void onStop(final Activity activity) {

    }

    public static void onResume(View ueContainerView, final Activity activity) {
        if (width <= 0 || height <= 0 || width < height) {
            return;
        }
        ueContainerView.getLayoutParams().width = width;
        ueContainerView.getLayoutParams().height = height;
    }

    private static void print(String msg) {
        Log.i("==XDGSDK==C", msg);
    }

    // ------JNI 回调-------
    public native static void nativeOnXDGSDKInitCompleted(boolean success, String result);

    // 0成功，1取消，2失败
    public native static void nativeOnXDGSDKShareCompleted(int code);

    public native static void nativeOnXDGSDKGetRegionInfoCompleted(String countryCode, String city, String timeZone,
            String locationInfoType);

    //获取安卓 XDG_info.jons配置        
    public native static void nativeOnXDGSDKGetXDGInfoJsonCompleted(String result);


}