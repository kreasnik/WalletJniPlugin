package com.android.walletport;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONObject;

public class WalletJniPort extends CordovaPlugin{
    private static final String TEST = "test";
    private JSONArray requestArgs;
    private CallbackContext callbackContext;
    /**
     * Constructor.
     */
    public WalletPort(){

    }

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) {
        this.callbackContext = callbackContext;
        this.requestArgs = args;

        if (action.equals(TEST)) {
            test();
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
