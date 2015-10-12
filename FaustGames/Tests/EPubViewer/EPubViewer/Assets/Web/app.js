/// <reference path="scripts/core.ts" />
/// <reference path="scripts/platformWeb.ts" />
/// <reference path="scripts/common.ts" />
/// <reference path="scripts/pagination.ts" />
/// <reference path="scripts/epub.ts" />
//var paginator: Pagination.Paginator = null;
var reader = null;
var lock = false;
window.onclick = function (ev) {
    //if (paginator == null) return;
    if (reader == null)
        return;
    if (lock)
        return;
    if (ev.clientX > (window.innerWidth * 0.5)) {
        if (reader.isNextPageAvaliable()) {
            lock = true;
            reader.openNextPage(function () {
                lock = false;
            });
        }
    }
    else {
        if (reader.isPrevPageAvaliable()) {
            lock = true;
            reader.openPrevPage(function () {
                lock = false;
            });
        }
    }
};
window.onload = function () {
    Core.Services.loader = new PlatformWeb.Loader();
    Core.Services.domParser = new Common.DefaultDomParser();
    Core.Services.domSynchronizer = new Common.DomSynchroniser();
    Core.Services.domModifier = new Common.DomModifier();
    Core.Services.domWalker = new Common.DomWalker();
    Core.Services.absolutBoundsDetector = new Common.AbsolutBoundsDetector();
    reader = new Epub.EpubReader();
    reader.loadBook("Content/QueryLanguage/OEBPS/", function (toc) {
        var location = new Core.EpubLocation();
        location.url = toc.urls[0];
        location.idFrom = location.idTo = Core.Config.firstPageId;
        reader.openLocation(location, function () {
        });
    });
    /*
    var baseUrl: string = "Content/buh12013/OEBPS/Text/";

    Core.Services.loader.load(baseUrl + "Calendar.html",(status: Core.LoadingStatus, fileText: string) => {
        var doc = Core.Services.domParser.parse(fileText, "text/html");
        var hasCallback = Core.Services.domModifier.updateImageSources(doc, baseUrl, loadingDoneCallback);
        Core.Services.domModifier.updateCssRefs(doc, baseUrl);
        Core.Services.domModifier.createSpansFormWords(doc);

        if (Core.Config.justifyParagraphs)
            Core.Services.domModifier.justifyParagraphs(doc);

        Core.Services.domSynchronizer.synchronize(document, doc);

        //Core.Services.domModifier.clearCss(document);

        Core.Services.domModifier.setTableWidth(document);
        Core.Services.domModifier.setTdWidth(document);
        Core.Services.domModifier.setImageSize(document);
        Core.Services.domModifier.addCss(document, "app.css");

        if (!hasCallback) {
            loadingDoneCallback();
        }
    });
    */
};
/// 9 
//# sourceMappingURL=app.js.map