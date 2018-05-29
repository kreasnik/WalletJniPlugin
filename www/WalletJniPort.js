cordova.define("cordova-plugin-walletjniport.walletjniport", function(require, exports, module) {
/**
 * cordova is available under the MIT License (2008).
 * See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) Matt Kane 2010
 * Copyright (c) 2011, IBM Corporation
 * Copyright (c) 2012-2017, Adobe Systems
 */


    var exec = cordova.require("cordova/exec");


    WalletJniPort.prototype.test = function (content) {

        exec(null, null, "test", [content]);
    };

    var walletJniPort = new WalletJniPort();
    module.exports = walletJniPort;

});
