/// <reference path="core.ts" />
var Epub;
(function (Epub) {
    var EpubLocation = Core.EpubLocation;
    var EpubToc = (function () {
        function EpubToc() {
            this.urls = [];
        }
        EpubToc.prototype.parse = function (bookUrl, doc) {
            this.bookUrl = bookUrl;
            this.parseNode(doc);
        };
        EpubToc.prototype.parseNode = function (node) {
            if (node.nodeName == "content") {
                this.addContent(node);
            }
            for (var i = 0; i < node.childNodes.length; i++) {
                this.parseNode(node.childNodes[i]);
            }
        };
        EpubToc.prototype.addContent = function (node) {
            for (var i = 0; i < node.attributes.length; i++) {
                if (node.attributes[i].name == "src") {
                    var value = node.attributes[i].value;
                    if (value.indexOf('#') == -1)
                        this.urls.push(this.bookUrl + value);
                }
            }
        };
        EpubToc.prototype.findNextUrl = function (url) {
            for (var i = 0; i < this.urls.length; i++) {
                if (url == this.urls[i]) {
                    if (i == (this.urls.length - 1))
                        return null;
                    return this.urls[i + 1];
                }
            }
            return null;
        };
        EpubToc.prototype.findPrevUrl = function (url) {
            for (var i = 0; i < this.urls.length; i++) {
                if (url == this.urls[i]) {
                    if (i == 0)
                        return null;
                    return this.urls[i - 1];
                }
            }
            return null;
        };
        return EpubToc;
    })();
    Epub.EpubToc = EpubToc;
    var EpubReader = (function () {
        function EpubReader() {
        }
        //urlIndex: number;
        EpubReader.prototype.loadBook = function (bookUrl, callback) {
            var _this = this;
            this.bookUrl = bookUrl;
            this.paginator = null;
            this.location = new EpubLocation();
            Core.Services.loader.load(this.bookUrl + Core.EpubFiles.toc, function (status, fileText) {
                var doc = Core.Services.domParser.parse(fileText, "application/xml");
                _this.toc = new EpubToc();
                _this.toc.parse(bookUrl, doc);
                callback(_this.toc);
            });
        };
        EpubReader.prototype.openLocation = function (location, callback) {
            this.paginator = null;
            this.location = location.createClone();
            this.openCurrentLocation(callback);
        };
        EpubReader.prototype.openNextPage = function (callback) {
            this.nextPage(callback);
        };
        EpubReader.prototype.openPrevPage = function (callback) {
            this.prevPage(callback);
        };
        EpubReader.prototype.openCurrentLocation = function (callback) {
            var _this = this;
            var url = this.location.url;
            this.currentBaseUrl = Core.Path.getDirectory(url);
            Core.Services.loader.load(url, function (status, fileText) {
                var doc = Core.Services.domParser.parse(fileText, "text/html");
                var hasCallback = Core.Services.domModifier.updateImageSources(doc, _this.currentBaseUrl, function () {
                    _this.loadingDoneCallback(doc, callback);
                });
                Core.Services.domModifier.updateCssRefs(doc, _this.currentBaseUrl);
                Core.Services.domModifier.createSpansFormWords(doc);
                if (Core.Config.justifyParagraphs)
                    Core.Services.domModifier.justifyParagraphs(doc);
                Core.Services.domSynchronizer.synchronize(document, doc);
                if (!hasCallback) {
                    _this.loadingDoneCallback(doc, callback);
                }
            });
        };
        EpubReader.prototype.getCurrentPageLocation = function () {
            return this.location.createClone();
        };
        EpubReader.prototype.getCurrentSelectionLocation = function () {
            return new EpubLocation(); // todo: get location from selection
        };
        EpubReader.prototype.isNextPageAvaliable = function () {
            if (this.paginator == null)
                return false;
            var nextUrl = this.toc.findNextUrl(this.location.url);
            if (nextUrl == null) {
                return this.paginator.pageIndex < (this.paginator.getPagesCount() - 1);
            }
            return true;
        };
        EpubReader.prototype.isPrevPageAvaliable = function () {
            if (this.paginator == null)
                return false;
            var prevUrl = this.toc.findPrevUrl(this.location.url);
            if (prevUrl == null) {
                return this.paginator.pageIndex > 0;
            }
            return true;
        };
        EpubReader.prototype.nextPage = function (callback) {
            if (this.paginator == null)
                return;
            if (this.paginator.pageIndex < (this.paginator.getPagesCount() - 1)) {
                this.paginator.displayPage(this.paginator.pageIndex + 1);
                this.location = this.paginator.calcLocation(this.location.url);
                callback();
            }
            else {
                var nextUrl = this.toc.findNextUrl(this.location.url);
                if (nextUrl == null)
                    return;
                this.location.url = nextUrl;
                this.location.idFrom = this.location.idTo = Core.Config.firstPageId;
                this.openCurrentLocation(callback);
            }
            /*
            if (this.urlIndex < (this.urls.length - 1)) {
                this.urlIndex++;
                this.openUrl();
            }
            */
        };
        EpubReader.prototype.prevPage = function (callback) {
            if (this.paginator == null)
                return;
            if (this.paginator.pageIndex > 0) {
                this.paginator.displayPage(this.paginator.pageIndex - 1);
                this.location = this.paginator.calcLocation(this.location.url);
                callback();
            }
            else {
                var prevUrl = this.toc.findPrevUrl(this.location.url);
                if (prevUrl == null)
                    return;
                this.location.url = prevUrl;
                this.location.idFrom = this.location.idTo = Core.Config.lastPageId;
                this.openCurrentLocation(callback);
            }
            /*
            if (this.urlIndex > 0) {
                this.urlIndex--;
                this.openUrl();
            }
            */
        };
        EpubReader.prototype.loadingDoneCallback = function (doc, callback) {
            var _this = this;
            //Core.Services.domModifier.clearCss(document);
            Core.Services.domModifier.updateFontSize(document);
            Core.Services.domModifier.setDisplayStyle(document);
            setTimeout(function () {
                Core.Services.domModifier.setTableWidth(document);
                Core.Services.domModifier.setTdWidth(document);
                setTimeout(function () {
                    Core.Services.domModifier.setImageSize(document);
                    Core.Services.domModifier.addCss(document, "app.css");
                    setTimeout(function () {
                        _this.paginator = new Pagination.Paginator();
                        _this.paginator.doPagination();
                        _this.paginator.displayId(_this.location.idFrom);
                        callback();
                    }, 100);
                }, 100);
            }, 100);
        };
        EpubReader.prototype.openUrl = function () {
            /*
            this.paginator = null;
            var url = this.urls[this.urlIndex];
            this.currentBaseUrl = Core.Path.getDirextory(url);
            Core.Services.loader.load(url,(status: Core.LoadingStatus, fileText: string) => {
                var doc = Core.Services.domParser.parse(fileText, "text/html");
                var hasCallback = Core.Services.domModifier.updateImageSources(doc, this.currentBaseUrl, () => {
                    this.loadingDoneCallback(doc);
                });
                Core.Services.domModifier.updateCssRefs(doc, this.currentBaseUrl);
                Core.Services.domModifier.createSpansFormWords(doc);

                if (Core.Config.justifyParagraphs)
                    Core.Services.domModifier.justifyParagraphs(doc);

                Core.Services.domSynchronizer.synchronize(document, doc);

                if (!hasCallback) {
                    this.loadingDoneCallback(doc);
                }
            });
            */
        };
        return EpubReader;
    })();
    Epub.EpubReader = EpubReader;
})(Epub || (Epub = {}));
//# sourceMappingURL=epub.js.map