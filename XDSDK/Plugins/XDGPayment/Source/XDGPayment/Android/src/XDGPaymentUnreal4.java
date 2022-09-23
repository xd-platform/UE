package com.xd;

import android.util.Log;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.tds.common.bridge.utils.BridgeJsonHelper;
import com.xd.intl.common.utils.TDSLogger;
import com.xd.intl.payment.XDGPayment;
import com.xd.intl.payment.callback.XDGPaymentCallback;
import com.xd.intl.payment.constant.Constants;
import com.xd.intl.payment.entities.PurchaseDetails;
import com.xd.intl.payment.entities.RefundDetails;
import com.xd.intl.payment.entities.TDSGlobalSkuDetails;
import com.xd.intl.payment.entities.XDGTransactionInfo;
import com.xd.intl.payment.wallet.XDGPaymentResult;
import android.content.SharedPreferences;

import org.json.JSONObject;
import android.os.Handler;
import android.os.Looper;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import android.text.TextUtils;

public class XDGPaymentUnreal4 {

    private static final String TAG = "XDGPayment Log:";

    public static void payWithProduct(String orderId, String productId, String roleId, String serverId, String ext) {
 
        if (TextUtils.isEmpty(orderId)) {
            orderId = UUID.randomUUID().toString();
        }

        final String oid = orderId;
        final String rid = roleId;
        print("queryWithProductIds 参数: orderId:" + oid + " productId:" + productId + " role:" + rid + " serverId:" + serverId + " ext:" + ext);

        XDGPayment.payWithProduct(oid, productId, rid, serverId, ext, new XDGPaymentCallback<Object>() {
            @Override
            public void onPaymentCallback(XDGPaymentResult result, Object data) {
                handlerOrderInfoToBridge(oid, productId, rid, serverId, result.code,
                        result.debugMessage);
            }
        });

    }

    public static void queryWithProductIds(String listJson) {
        print("queryWithProductIds打印结果: " + listJson);

        String[] productIds = getProductIdList(listJson);
        if (productIds != null && productIds.length > 0) {
            XDGPayment.queryWithProductIds(Arrays.asList(productIds),
                    new XDGPaymentCallback<List<TDSGlobalSkuDetails>>() {
                        @Override
                        public void onPaymentCallback(XDGPaymentResult result, List<TDSGlobalSkuDetails> data) {
                            Map<String, Object> params = new HashMap<>();
                            params.put("products", data);
                            if (result.code == Constants.PaymentResponseCode.OK) {
                                String json = BridgeJsonHelper.object2JsonString(params);
                                nativeOnXDGSDKQueryProductIdsSucceed(json);
                            } else {
                                nativeOnXDGSDKQueryProductIdsFailed(result.code, result.debugMessage);
                            }
                        }
                    });
        }
    }

    private static String[] getProductIdList(String listJson) {
        try {
            JSONObject object = new JSONObject(listJson);
            List<String> list = new Gson().fromJson(object.getString("list"), new TypeToken<List<String>>() {
            }.getType());
            String[] array = new String[list.size()];
            return list.toArray(array);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static void queryRestoredPurchases() {
        print("queryRestoredPurchases");
        XDGPayment.queryRestoredPurchases(new XDGPaymentCallback<List<XDGTransactionInfo>>() {
            @Override
            public void onPaymentCallback(XDGPaymentResult result, List<XDGTransactionInfo> data) {
                Map<String, Object> resultParams = new HashMap<>();
                resultParams.put("transactions", data);
                if (result.code == Constants.PaymentResponseCode.OK) {
                    String json = BridgeJsonHelper.object2JsonString(resultParams);
                    nativeOnXDGSDKQueryRestoredPurchasesSucceed(json);
                    print("结果：" + json);
                } else {
                    nativeOnXDGSDKQueryRestoredPurchasesFailed(result.code, result.debugMessage);
                }
            }
        });
    }

    public static void payWithWeb(  String orderId,
                                    String productId,
                                    String productName,
                                    float payAmount,
                                    String roleId,
                                    String serverId,
                                    String extras) {


        if (TextUtils.isEmpty(orderId)) {
            orderId = UUID.randomUUID().toString();
        }

        final String rid = roleId;
        print("网页支付参数:   orderId:" + orderId + " productId:" + productId + " productName: " + productName + " payAmount: " + payAmount +" rid: " + rid +" serverId: " + serverId +" extras: " + extras);
        
        XDGPayment.payWithWeb(orderId, productId, productName, payAmount, rid, serverId, extras, new XDGPaymentCallback<Object>() {
            @Override
            public void onPaymentCallback(XDGPaymentResult result, Object data) {
                nativeOnXDGSDKPayWithWebCompleted(serverId, rid, result.code, result.debugMessage);
                print("网页支付结果:  code = " + result.code + "  message: " + result.debugMessage);
            }
        });
    }

    public static void checkRefundStatus() {
        XDGPayment.checkRefundStatus(new XDGPaymentCallback<Object>() {
            @Override
            public void onPaymentCallback(XDGPaymentResult result, Object data) {
                convertRefundCheckResultToBridge(result, data);
            }
        });
    }

    public static void checkRefundStatusWithUI() {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                XDGPayment.checkRefundStatusWithUI(new XDGPaymentCallback<Object>() {
                    @Override
                    public void onPaymentCallback(XDGPaymentResult result, Object data) {
                        convertRefundCheckResultToBridge(result, data);
                    }
                });

            }
        });
    }

    private static void handlerOrderInfoToBridge(String orderId, String productId, String roleId, String serverId,
            int code, String message) {
        print("结果: code = " + code + "   message = " + message);
        if (code == Constants.PaymentResponseCode.OK) {
            nativeOnXDGSDKPaymentSucceed(orderId, productId, serverId, roleId);

        } else {
            nativeOnXDGSDKPaymentFailed(code, message);
        }
    }

    private static void convertRefundCheckResultToBridge(XDGPaymentResult result, Object data) {
        if (result.code == Constants.PaymentResponseCode.OK) {
            if (data instanceof List) {
                Map<String, Object> successParam = new HashMap<>();
                try {
                    List<RefundDetails> refundDetailsList = (List<RefundDetails>) data;
                    successParam.put("list", refundDetailsList);
                } catch (Exception e) {
                    TDSLogger.e(e.getMessage());
                }

                String json = BridgeJsonHelper.object2JsonString(successParam);
                nativeOnXDGSDKCheckRefundStatusSucceed(json);
                print("成功： " + json);
            }

        } else {
            nativeOnXDGSDKCheckRefundStatusFailed(result.code, result.debugMessage);
            print("失败了 code = " + result.code + "  message = " + result.debugMessage);
        }
    }



    public static void restorePurchase(String restorePurchase, String productId, String orderId, String roleId, String serverId, String ext) {
        Log.i(TAG, "restorePurchase:" + restorePurchase + " productId:" + productId + " role:" + roleId + " serverId:"
                + serverId + " ext:" + ext);

        XDGTransactionInfo info = new XDGTransactionInfo();
        info.purchaseToken = restorePurchase;
        info.productId = productId;
        info.orderId = orderId;

        PurchaseDetails replenishmentOrderPurchaseDetails = new PurchaseDetails();
        replenishmentOrderPurchaseDetails.purchaseToken = restorePurchase;
        replenishmentOrderPurchaseDetails.productId = productId;
        info.replenishmentOrderPurchaseDetails = replenishmentOrderPurchaseDetails;
        
        XDGPayment.restorePurchase(info, orderId, productId, roleId, serverId, ext, new XDGPaymentCallback<Object>() {
            @Override
            public void onPaymentCallback(XDGPaymentResult result, Object data) {
                handlerOrderInfoToBridge(orderId, productId, roleId, serverId, result.code, result.debugMessage);

            }
        });
    }

    private static void print(String msg) {
        Log.i("==XDGSDK==P", msg);
    }

    // ------JNI 回调-------
    public native static void nativeOnXDGSDKPaymentSucceed(String orderId, String productId, String serverId,
            String roleId);

    public native static void nativeOnXDGSDKPaymentFailed(int code, String msg);

    public native static void nativeOnXDGSDKQueryProductIdsSucceed(String resultJson);

    public native static void nativeOnXDGSDKQueryProductIdsFailed(int code, String msg);

    public native static void nativeOnXDGSDKQueryRestoredPurchasesSucceed(String resultJson);

    public native static void nativeOnXDGSDKQueryRestoredPurchasesFailed(int code, String msg);

    public native static void nativeOnXDGSDKPayWithWebCompleted(String serverId, String roleId, int code, String errorMsg);

    public native static void nativeOnXDGSDKCheckRefundStatusSucceed(String resultJson);

    public native static void nativeOnXDGSDKCheckRefundStatusFailed(int code, String msg);

}