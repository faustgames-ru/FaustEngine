/// <reference path="core.ts" />
var PlatformWeb;
(function (PlatformWeb) {
    var LoadingStatus = Core.LoadingStatus;
    var Loader = (function () {
        function Loader() {
        }
        Loader.prototype.load = function (fileName, resultCallback) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", fileName, true);
            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4) {
                    if ((xhr.status == 200)) {
                        resultCallback(0 /* Ok */, xhr.responseText);
                    }
                    else {
                        resultCallback(1 /* Failed */, xhr.responseText);
                    }
                }
            };
            xhr.send(null);
        };
        return Loader;
    })();
    PlatformWeb.Loader = Loader;
})(PlatformWeb || (PlatformWeb = {}));
//# sourceMappingURL=platformWeb.js.map