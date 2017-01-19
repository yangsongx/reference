var exec = require('cordova/exec');
//var allApis = {};

exports.coolMethod = function(arg0, success, error) {
    exec(success, error, "MyVoice", "coolMethod", [arg0]);
};


//module.exports = allApis;
