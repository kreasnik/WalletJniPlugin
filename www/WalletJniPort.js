//cordova.define("cordova-plugin-walletjniport.walletjniport", function(require, exports, module) {
/**
 * cordova is available under the MIT License (2008).
 * See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) Matt Kane 2010
 * Copyright (c) 2011, IBM Corporation
 * Copyright (c) 2012-2017, Adobe Systems
 */


    var exec = cordova.require("cordova/exec");
    var WalletJniPort = function(){};

    //设置密码
    WalletJniPort.prototype.setPassphrase = function(passphrase, successCallback, errorCallback){
        console.log("WalletJniPort -- setPassphrase");
        if (errorCallback == null) {
            errorCallback = function () {
            };
        }

        if (typeof errorCallback != "function") {
            console.log("WalletJniPort.setPassphrase failure: failure parameter not a function");
            return;
        }

        if (typeof successCallback != "function") {
            console.log("WalletJniPort.setPassphrase failure: success callback parameter must be a function");
            return;
        }
        exec(successCallback, errorCallback, "WalletJniPort", "setPassphrase", [{"passphrase": passphrase}]);
    };
    //创建助记词
    WalletJniPort.prototype.createMnemonic = function(passphrase, language, number, successCallback, errorCallback){
        console.log("WalletJniPort -- createMnemonic");
        if (errorCallback == null) {
            errorCallback = function () {
            };
        }

        if (typeof errorCallback != "function") {
            console.log("WalletJniPort.createMnemonic failure: failure parameter not a function");
            return;
        }

        if (typeof successCallback != "function") {
            console.log("WalletJniPort.createMnemonic failure: success callback parameter must be a function");
            return;
        }
        exec(
            successCallback,
            errorCallback,
            "WalletJniPort",
            "createMnemonic",
            [{"passphrase": passphrase, "language": language, "number": number}]
        );
    };
    //恢复助记词
    WalletJniPort.prototype.recoveryMnemonic = function(passphrase, language, number, mnemonic, successCallback, errorCallback){
        console.log("WalletJniPort -- recoveryMnemonic");
        if (errorCallback == null) {
            errorCallback = function () {
            };
        }

        if (typeof errorCallback != "function") {
            console.log("WalletJniPort.recoveryMnemonic failure: failure parameter not a function");
            return;
        }

        if (typeof successCallback != "function") {
            console.log("WalletJniPort.recoveryMnemonic failure: success callback parameter must be a function");
            return;
        }
        exec(
            successCallback,
            errorCallback,
            "WalletJniPort",
            "recoveryMnemonic",
            [{"passphrase": passphrase, "language": language, "number": number, "mnemonic": mnemonic}]
        );
    };
    //派生
    WalletJniPort.prototype.derive = function(passphrase, path, deriveAlgoId, signAlgoId, number, successCallback, errorCallback){
        console.log("WalletJniPort -- derive");
        if (errorCallback == null) {
            errorCallback = function () {
            };
        }

        if (typeof errorCallback != "function") {
            console.log("WalletJniPort.derive failure: failure parameter not a function");
            return;
        }

        if (typeof successCallback != "function") {
            console.log("WalletJniPort.derive failure: success callback parameter must be a function");
            return;
        }
        exec(
            successCallback,
            errorCallback,
            "WalletJniPort",
            "derive",
            [{
                "passphrase": passphrase,
                "path": path,
                "deriveAlgoId": deriveAlgoId,
                "signAlgoId": signAlgoId,
                "number": number
            }]
        );
    };
    //签名
    WalletJniPort.prototype.sign = function(passphrase, path, deriveAlgoId, signAlgoId, transhash, number, successCallback, errorCallback){
        console.log("WalletJniPort -- sign");
        if (errorCallback == null) {
            errorCallback = function () {
            };
        }

        if (typeof errorCallback != "function") {
            console.log("WalletJniPort.sign failure: failure parameter not a function");
            return;
        }

        if (typeof successCallback != "function") {
            console.log("WalletJniPort.sign failure: success callback parameter must be a function");
            return;
        }
        exec(
            successCallback,
            errorCallback,
            "WalletJniPort",
            "sign",
            [{
                "passphrase": passphrase,
                "path": path,
                "deriveAlgoId": deriveAlgoId,
                "signAlgoId": signAlgoId,
                "number": number,
                "transhash": transhash
            }]
        );
    };
    //设置密码
    WalletJniPort.prototype.deleteWallet = function(passphrase, successCallback, errorCallback){
        console.log("WalletJniPort -- deleteWallet");
        if (errorCallback == null) {
            errorCallback = function () {
            };
        }

        if (typeof errorCallback != "function") {
            console.log("WalletJniPort.deleteWallet failure: failure parameter not a function");
            return;
        }

        if (typeof successCallback != "function") {
            console.log("WalletJniPort.deleteWallet failure: success callback parameter must be a function");
            return;
        }
        exec(successCallback, errorCallback, "WalletJniPort", "deleteWallet", [{"passphrase": passphrase}]);
    };
    //测试
    WalletJniPort.prototype.test = function(content){
        console.log("js test start");
        exec(null, null, "WalletJniPort", "test", [content]);
    };

    var walletJniPort = new WalletJniPort();
    module.exports = walletJniPort;

//});
