package com.android.walletport;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;
import android.util.Log;

public class WalletJniPort extends CordovaPlugin{
    private static final String TAG = "WalletJniPort";
    private static final String TEST = "test";
    private static final String SETPASSPHRASE = "setPassphrase";
    private static final String CREATEMNEMONIC = "createMnemonic";
    private static final String RECOVERYMNEMONIC = "recoveryMnemonic";
    private static final String DERIVE = "derive";
    private static final String SIGN = "sign";
    private static final String DELETEWALLET = "deleteWallet";

    private static final String PASSPHRASE = "passphrase";
    private static final String LANGUAGE = "language";
    private static final String NUMBER = "number";
    private static final String MNEMONIC = "mnemonic";
    private static final String PATH = "path";
    private static final String DERIVEALGOID = "deriveAlgoId";
    private static final String SIGNALGOID = "signAlgoId";
    private static final String TRANSHASH = "transhash";

    private static final String PUBKEY = "pubkey";
    private static final String SIGNHASH = "signhash";
    private JSONArray requestArgs;
    private CallbackContext callbackContext;
    /**
     * Constructor.
     */
    public WalletJniPort(){
        wallet_init();
    }

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        this.callbackContext = callbackContext;
        this.requestArgs = args;
        Log.d(TAG, "action = " + action);
        if (action.equals(TEST)) {
            cordova.getThreadPool().execute(new Runnable() {
                public void run() {
                    Log.d(TAG, "WalletJniPort test start");
                    test();
                }
            });
        }else if(action.equals(SETPASSPHRASE)){
            JSONObject obj = args.optJSONObject(0);
            if(obj != null){
                String passphrase = obj.optString(PASSPHRASE);
                setPassphrase(passphrase);
            }else {
                callbackContext.error("User did not specify data to setPassphrase");
                return true;
            }
        }else if(action.equals(CREATEMNEMONIC)){
            JSONObject obj = args.optJSONObject(0);
            if(obj != null){
                String passphrase = obj.optString(PASSPHRASE);
                String language = obj.optString(LANGUAGE);
                String number = obj.optString(NUMBER);
                createMnemonic(passphrase, language, number);
            }else {
                callbackContext.error("User did not specify data to createMnemonic");
                return true;
            }
        }else if(action.equals(RECOVERYMNEMONIC)){
            JSONObject obj = args.optJSONObject(0);
            if(obj != null){
                String passphrase = obj.optString(PASSPHRASE);
                String language = obj.optString(LANGUAGE);
                String number = obj.optString(NUMBER);
                String mnemonic = obj.optString(MNEMONIC);
                recoveryMnemonic(passphrase, language, number, mnemonic);
            }else {
                callbackContext.error("User did not specify data to recoveryMnemonic");
                return true;
            }
        }else if(action.equals(DERIVE)){
            JSONObject obj = args.optJSONObject(0);
            if(obj != null){
                String passphrase = obj.optString(PASSPHRASE);
                String path = obj.optString(PATH);
                String deriveAlgoId = obj.optString(DERIVEALGOID);
                String signAlgoId = obj.optString(SIGNALGOID);
                String number = obj.optString(NUMBER);
                derive(passphrase, path, deriveAlgoId, signAlgoId, number);
            }else {
                callbackContext.error("User did not specify data to derive");
                return true;
            }
        }else if(action.equals(SIGN)){
            JSONObject obj = args.optJSONObject(0);
            if(obj != null){
                String passphrase = obj.optString(PASSPHRASE);
                String path = obj.optString(PATH);
                String deriveAlgoId = obj.optString(DERIVEALGOID);
                String signAlgoId = obj.optString(SIGNALGOID);
                String number = obj.optString(NUMBER);
                String transhash = obj.optString(TRANSHASH);
                sign(passphrase, path, deriveAlgoId, signAlgoId, number, transhash);
            }else {
                callbackContext.error("User did not specify data to sign");
                return true;
            }
        }else if(action.equals(DELETEWALLET)){
            JSONObject obj = args.optJSONObject(0);
            if(obj != null){
                String passphrase = obj.optString(PASSPHRASE);
                deleteWallet(passphrase);
            }else {
                callbackContext.error("User did not specify data to deleteWallet");
                return true;
            }
        }else {
            return false;
        }
        return true;
    }

    public void setPassphrase(final String passphrase){
        cordova.getThreadPool().execute(new Runnable() {
            public void run() {
                int ret = set_passphrase(passphrase);
                if(ret == 1){
                    WalletJniPort.this.callbackContext.success("ok");
                }else{
                    WalletJniPort.this.callbackContext.error("setPassphrase failed");
                }
            }
        });
    }

    public void createMnemonic(final String passphrase, final String language, final String number){
        cordova.getThreadPool().execute(new Runnable() {
            public void run() {
                int ret = create_mnemonic(passphrase, language, Integer.parseInt(number));
                if(ret == 1){
                    WalletJniPort.this.callbackContext.success("ok");
                }else{
                    WalletJniPort.this.callbackContext.error("createMnemonic failed");
                }
            }
        });
    }

    public void recoveryMnemonic(final String passphrase, final String language, final String number, final String mnemonic){
        cordova.getThreadPool().execute(new Runnable() {
            public void run() {
                int ret = recovery_mnemonic(passphrase, language, Integer.parseInt(number), mnemonic);
                if(ret == 1){
                    WalletJniPort.this.callbackContext.success("ok");
                }else{
                    WalletJniPort.this.callbackContext.error("recovery_mnemonic failed");
                }
            }
        });
    }

    public void derive(final String passphrase, final String path, final String deriveAlgoId, final String signAlgoId, final String number){
        cordova.getThreadPool().execute(new Runnable() {
            public void run() {
                start_derive(passphrase, path, Integer.parseInt(deriveAlgoId), Integer.parseInt(signAlgoId), Integer.parseInt(number), "deriveCallback");
            }
        });
    }

    public void deriveCallback(byte[] pubkey){
        String xpubkey = byteArrayToHexStr(pubkey);
        Log.d(TAG, "WalletJniPort deriveCallback xpubkey = " + xpubkey);
        if(xpubkey != null) {
            JSONObject obj = new JSONObject();
            try {
                obj.put(PUBKEY, xpubkey);
            } catch (JSONException e) {
                Log.d(TAG, "This should never happen");
            }
            this.callbackContext.success(obj);
        }else{
            this.callbackContext.error("deriveCallback failed");
        }
    }

    public void sign(final String passphrase, final String path, final String deriveAlgoId, final String signAlgoId, final String number, final String transhash){
        cordova.getThreadPool().execute(new Runnable() {
            public void run() {
                start_sign(passphrase, path, Integer.parseInt(deriveAlgoId), Integer.parseInt(signAlgoId), Integer.parseInt(number), transhash, "signCallback");
            }
        });
    }

    public void signCallback(byte[] pubkey, byte[] signhash){
        String xpubkey = byteArrayToHexStr(pubkey);
        Log.d(TAG, "WalletJniPort signCallback xpubkey = " + xpubkey);
        String xsignhash = byteArrayToHexStr(signhash);
        Log.d(TAG, "WalletJniPort signCallback xsignhash = " + xsignhash);
        if(xpubkey != null && xsignhash != null) {
            JSONObject obj = new JSONObject();
            try {
                obj.put(PUBKEY, xpubkey);
                obj.put(SIGNHASH, xsignhash);
            } catch (JSONException e) {
                Log.d(TAG, "This should never happen");
            }
            this.callbackContext.success(obj);
        }else{
            this.callbackContext.error("deriveCallback failed");
        }
    }

    public void deleteWallet(final String passphrase){
        cordova.getThreadPool().execute(new Runnable() {
            public void run() {
                int ret = delete_wallet(passphrase);
                if(ret == 1){
                    WalletJniPort.this.callbackContext.success("ok");
                }else{
                    WalletJniPort.this.callbackContext.error("delete_wallet failed");
                }
            }
        });
    }

    public static String byteArrayToHexStr(byte[] byteArray) {
        if (byteArray == null){
            return null;
        }
        char[] hexArray = "0123456789abcdef".toCharArray();
        char[] hexChars = new char[byteArray.length * 2];
        for (int j = 0; j < byteArray.length; j++) {
            int v = byteArray[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }
    //JNI
    public native void test();
    public native int wallet_init();
    public native int wallet_deinit();
    public native int set_passphrase(String passphrase);
    public native int create_mnemonic(String passphrase, String language, int number);
    public native int recovery_mnemonic(String passphrase, String language, int number, String mnemonic);
    public native int start_derive(String passphrase, String path, int deriveAlgoId, int signAlgoId, int number, String callback);
    public native int start_sign(String passphrase, String path, int deriveAlgoId, int signAlgoId, int number, String transhash, String callback);
    public native int delete_wallet(String passphrase);
    static {
        System.loadLibrary("wallet_port");
    }
}
