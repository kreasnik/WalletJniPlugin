package com.android.walletport;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;

public class WalletJniPort extends CordovaPlugin{
    private static final String TEST = "test";
    private JSONArray requestArgs;
    private CallbackContext callbackContext;
    /**
     * Constructor.
     */
    public WalletJniPort(){

    }

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        this.callbackContext = callbackContext;
        this.requestArgs = args;
        Log.d("xuejin", "action = " + action);
        if (action.equals(TEST)) {
            cordova.getThreadPool().execute(new Runnable() {
                public void run() {
                    Log.d("xuejin", "WalletJniPort test start");
                    test();
                }
            });
        } else {
            return false;
        }
        return true;
    }


    //JNI
    public native void test();
    static {
        System.loadLibrary("wallet_port");
    }
}
