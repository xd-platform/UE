package com.xd;

import android.text.TextUtils;
import android.util.Log;
import android.os.Handler;
import android.os.Looper;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import android.content.SharedPreferences;

import com.tds.common.bridge.BridgeCallback;
import com.tds.common.bridge.IBridgeService;
import com.tds.common.bridge.annotation.BridgeMethod;
import com.tds.common.bridge.annotation.BridgeParam;
import com.tds.common.bridge.annotation.BridgeService;

import com.taptap.reactor.rxandroid.schedulers.AndroidSchedulers;
import com.taptap.reactor.schedulers.Schedulers;
import com.tds.common.bridge.BridgeCallback;
import com.tds.common.bridge.utils.BridgeJsonHelper;
import com.xd.intl.account.XDGAccount;
import com.xd.intl.account.entity.LoginEntriesConfig;
import com.xd.intl.account.entity.LoginEntryType;
import com.xd.intl.account.entity.TapSessionToken;
import com.xd.intl.account.impl.TDSGlobalAccountComponent;
import com.xd.intl.account.utils.LoginEntriesHelper;
import com.xd.intl.account.utils.XDLoginTokenManager;
import com.xd.intl.common.base.AbstractSubscriber;
import com.xd.intl.common.base.XDGError;
import com.xd.intl.common.bean.XDGUser;
import com.xd.intl.common.callback.Callback;
import com.xd.intl.common.callback.XDGBindResultCallback;
import com.xd.intl.common.callback.XDGUserStatusChangeCallback;
import com.xd.intl.common.utils.EnvHelper;
import com.xd.intl.common.utils.TDSLogger;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONObject;


public class XDGAccountUnreal4 {

    private static long lastClick = 0; // 防止重复点击
    private static int gapTime = 1000; // 点击间隔毫秒

    public static void login(final String loginTypeList) {
        if (System.currentTimeMillis() - lastClick > gapTime) {
            print("点击 Login: " + loginTypeList);
            lastClick = System.currentTimeMillis();

            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override
                public void run() {
                    String[] loginNameArray = getLoginTypeList(loginTypeList);

                    if (loginNameArray != null && loginNameArray.length > 0) {
                        LoginEntriesConfig.Builder builder = new LoginEntriesConfig.Builder();
                        for (String name : loginNameArray) {
                            LoginEntryType loginEntryType = null;
                            try {
                                loginEntryType = LoginEntriesHelper.getLoginEntryTypeByName(name.trim());
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                            if (loginEntryType != null) {
                                builder.addLoginEntry(loginEntryType);
                            }
                        }

                        XDGAccount.login(builder.build(), new Callback<XDGUser>() {
                            @Override
                            public void onCallback(XDGUser tdsGlobalUser, XDGError xdgError) {
                                constructorUserForBridge(tdsGlobalUser, xdgError, true);
                            }
                        });
                    }else{
                        print("登录参数是空：" + loginTypeList);
                    }
                }
            });
        }
    }


    private static String[] getLoginTypeList(String listJson) {
        try {
            JSONObject object = new JSONObject(listJson);
            List<String> list = new Gson().fromJson(object.getString("list"), new TypeToken<List<String>>() {}.getType());
            String[] array = new String[list.size()];
            return list.toArray(array);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private static void constructorUserForBridge(XDGUser xdgUser, XDGError tdsServerError, boolean isLogin) {
        if (xdgUser != null) {
            Map<String, Object> tdsUserParams = new HashMap<>(4);
            tdsUserParams.put("userId", xdgUser.getId());
            tdsUserParams.put("name", xdgUser.getName());
            tdsUserParams.put("loginType",
                    LoginEntriesHelper.getLoginEntryTypeByType(xdgUser.getLoginType()).getName());
            List<String> boundAccounts;
            if (xdgUser.getBoundAccounts() == null) {
                boundAccounts = new ArrayList<String>();
            } else {
                boundAccounts = xdgUser.getBoundAccounts();
            }
            tdsUserParams.put("boundAccounts", boundAccounts);
            Map<String, Object> tokenParams = new HashMap<>(5);
            tokenParams.put("kid", xdgUser.getAccessToken().getKid());
            tokenParams.put("tokenType", xdgUser.getAccessToken().getTokenType());
            tokenParams.put("macKey", xdgUser.getAccessToken().getMacKey());
            tokenParams.put("macAlgorithm", xdgUser.getAccessToken().getMacAlgorithm());
            tdsUserParams.put("token", tokenParams);

            String userJson = BridgeJsonHelper.object2JsonString(tdsUserParams);
            if (isLogin) { // 登录成功
                nativeOnXDGSDKLoginSucceed(userJson);

            } else {// 获取用户成功
                nativeOnXDGSDKGetUserSucceed(userJson);
            }
            print("成功：" + userJson);

        } else if (tdsServerError != null) {
            int code = tdsServerError.getCode();
            String msg = tdsServerError.getMessage();

            if (isLogin) { // 登录失败
                nativeOnXDGSDKLoginFailed(code, msg, BridgeJsonHelper.object2JsonString(tdsServerError.getErrorDataMap()));

            } else {// 获取用户失败
                nativeOnXDGSDKGetUserFailed(code, msg);
            }

            print("邮箱失败: code = " + code + "  msg = " + msg + " data: " + BridgeJsonHelper.object2JsonString(tdsServerError.getErrorDataMap()));
        }
    }

    public static void loginByType(String loginType) {
        if (System.currentTimeMillis() - lastClick > gapTime) {
            print("点击  LoginByType");
            lastClick = System.currentTimeMillis();

            new Handler(Looper.getMainLooper()).post(new Runnable() {
                @Override
                public void run() {
                    if (!TextUtils.isEmpty(loginType)) {

                        XDGAccount.loginByType(LoginEntriesHelper.getLoginTypeForLogin(loginType),
                                new Callback<XDGUser>() {
                                    @Override
                                    public void onCallback(XDGUser user, XDGError tdsServerError) {
                                        constructorUserForBridge(user, tdsServerError, true);
                                    }
                                });
                    }
                }
            });
        }
    }

    public static void addUserStatusChangeCallback() {
        print("点击  AddUserStatusChangeCallback");

        XDGAccount.addUserStatusChangeCallback(new XDGUserStatusChangeCallback() {
            @Override
            public void userStatusChange(int code, String message) {
                nativeOnXDGSDKUserStateChanged(code, message);
                print("changed status, code = " + code + "  msg = " + message);
            }
        });
    }

    public static void getUser() {
        print("点击  GetUser");
        XDGAccount.getUser(new Callback<XDGUser>() {
            @Override
            public void onCallback(XDGUser user, XDGError tdsServerError) {
                constructorUserForBridge(user, tdsServerError, false);
            }
        });
    }

    public static void openUserCenter() {
        print("点击  OpenUserCenter");
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                XDGAccount.openUserCenter();
            }
        });
    }

    public static void logout() {
        print("点击  Logout");
        XDGAccount.logout();
    }

    public static void accountCancellation() {
        XDGAccount.accountCancellation();
    }

    public static boolean isTokenActiveWithType(String loginTypeName) {
        return XDLoginTokenManager.isTokenActiveWithType(LoginEntriesHelper.getLoginTypeForLogin(loginTypeName));
    }


    public static void bindByType(final String loginTypeName) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                print("绑定类型：" + loginTypeName);
                XDGAccount.bindByType(LoginEntriesHelper.getLoginTypeForLogin(loginTypeName), new XDGBindResultCallback() {
                    @Override
                    public void onBindResult(boolean success, XDGError xdgError) {
                        int code = 1;
                        String msg = "success";
    
                        if (!success && xdgError != null) {
                            code = xdgError.getCode();
                            msg = xdgError.getDetailDebugMsg();
                        }
    
                        nativeOnXDGSDKBindByTypeCompleted(success ,code, msg);   
                    }
                });    
            }
        });
    }


    private static void print(String msg) {
        Log.i("==XDGSDK==A", msg);
    }

    // ------JNI 回调-------
    public native static void nativeOnXDGSDKBindByTypeCompleted(boolean success, int code, String msg);

    public native static void nativeOnXDGSDKLoginSucceed(String userJson);

    public native static void nativeOnXDGSDKLoginFailed(int code, String msg, String data);

    public native static void nativeOnXDGSDKGetUserSucceed(String userJson);

    public native static void nativeOnXDGSDKGetUserFailed(int code, String msg);

    public native static void nativeOnXDGSDKUserStateChanged(int code, String msg);

}