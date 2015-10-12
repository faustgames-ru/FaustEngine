/// <reference path="core.ts" />

module Epub {
    import EpubLocation = Core.EpubLocation;

    export class EpubToc {
        public urls: string[] = []
        public bookUrl: string;
        parse(bookUrl: string, doc: Document) {
            this.bookUrl = bookUrl;
            this.parseNode(doc);
        }

        public parseNode(node: Node) {
            if (node.nodeName == "content") {
                this.addContent(node);
            }
            for (var i = 0; i < node.childNodes.length; i++) {
                this.parseNode(node.childNodes[i]);
            }
        }

        public addContent(node: Node) {
            for (var i = 0; i < node.attributes.length; i++) {
                if (node.attributes[i].name == "src") {
                    var value: string = node.attributes[i].value;
                    if (value.indexOf('#') == -1)
                        this.urls.push(this.bookUrl + value);
                }
            }
        }

        findNextUrl(url: string): string {
            for (var i = 0; i < this.urls.length; i++) {
                if (url == this.urls[i]) {
                    if (i == (this.urls.length - 1))
                        return null;
                    return this.urls[i + 1];
                }
            }
            return null;
        }

        findPrevUrl(url: string): string {
            for (var i = 0; i < this.urls.length; i++) {
                if (url == this.urls[i]) {
                    if (i == 0)
                        return null;
                    return this.urls[i - 1];
                }
            }
            return null;
        }

    }

    export interface IEpubReaderLoadCallback {
        (toc: EpubToc): void;
    }

    export interface IEpubReaderOpenCallback {
        (): void;
    }

    export interface IEpubReader {
        loadBook(bookUrl: string, callback: IEpubReaderLoadCallback): void;
        openLocation(location: EpubLocation, callback: IEpubReaderOpenCallback): void;
        openNextPage(callback: IEpubReaderOpenCallback): void;
        openPrevPage(callback: IEpubReaderOpenCallback): void;
        getCurrentPageLocation(): EpubLocation;
        getCurrentSelectionLocation(): EpubLocation;
        isNextPageAvaliable(): boolean;
        isPrevPageAvaliable(): boolean;
    }

    export class EpubReader implements IEpubReader {
        location: EpubLocation;
        //bookmarkId: BookmarkId;
        bookUrl: string;
        currentBaseUrl: string;
        toc: EpubToc;
        //urls: string[] = [];
        paginator: Pagination.Paginator;
        //urlIndex: number;

        public loadBook(bookUrl: string, callback: IEpubReaderLoadCallback): void {
            this.bookUrl = bookUrl;
            this.paginator = null;
            this.location = new EpubLocation();
            Core.Services.loader.load(this.bookUrl + Core.EpubFiles.toc,(status: Core.LoadingStatus, fileText: string) => {
                var doc = Core.Services.domParser.parse(fileText, "application/xml");
                this.toc = new EpubToc();
                this.toc.parse(bookUrl, doc);
                callback(this.toc);
            });

        }

        public openLocation(location: EpubLocation, callback: IEpubReaderOpenCallback): void {
            this.paginator = null;
            this.location = location.createClone();
            this.openCurrentLocation(callback);
        }

        public openNextPage(callback: IEpubReaderOpenCallback): void {
            this.nextPage(callback);
        }

        public openPrevPage(callback: IEpubReaderOpenCallback): void {
            this.prevPage(callback);
        }


        public openCurrentLocation(callback: IEpubReaderOpenCallback): void {
            var url = this.location.url;
            this.currentBaseUrl = Core.Path.getDirectory(url);
            Core.Services.loader.load(url,(status: Core.LoadingStatus, fileText: string) => {
                var doc = Core.Services.domParser.parse(fileText, "text/html");
                var hasCallback = Core.Services.domModifier.updateImageSources(doc, this.currentBaseUrl,() => {
                    this.loadingDoneCallback(doc, callback);
                });
                Core.Services.domModifier.updateCssRefs(doc, this.currentBaseUrl);
                Core.Services.domModifier.createSpansFormWords(doc);

                if (Core.Config.justifyParagraphs)
                    Core.Services.domModifier.justifyParagraphs(doc);

                Core.Services.domSynchronizer.synchronize(document, doc);

                if (!hasCallback) {
                    this.loadingDoneCallback(doc, callback);
                }
            });
        }

        public getCurrentPageLocation(): EpubLocation {
            return this.location.createClone();
        }

        public getCurrentSelectionLocation(): EpubLocation {
            return new EpubLocation(); // todo: get location from selection
        }

        public isNextPageAvaliable(): boolean {
            if (this.paginator == null) return false;
            var nextUrl = this.toc.findNextUrl(this.location.url);
            if (nextUrl == null) {
                return this.paginator.pageIndex < (this.paginator.getPagesCount() - 1);
            }
            return true;
        }

        public isPrevPageAvaliable(): boolean {
            if (this.paginator == null) return false;
            var prevUrl = this.toc.findPrevUrl(this.location.url);
            if (prevUrl == null) {
                return this.paginator.pageIndex > 0;
            }
            return true;
        }


        public nextPage(callback: IEpubReaderOpenCallback): void {
            if (this.paginator == null) return;
            if (this.paginator.pageIndex < (this.paginator.getPagesCount() - 1)) {
                this.paginator.displayPage(this.paginator.pageIndex + 1);
                this.location = this.paginator.calcLocation(this.location.url);
                callback();
            } else {
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
        }

        public prevPage(callback: IEpubReaderOpenCallback): void {
            if (this.paginator == null) return;
            if (this.paginator.pageIndex > 0) {
                this.paginator.displayPage(this.paginator.pageIndex - 1);
                this.location = this.paginator.calcLocation(this.location.url);
                callback();

            } else {
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
        }

        public loadingDoneCallback(doc: Document, callback: IEpubReaderOpenCallback): void {
            //Core.Services.domModifier.clearCss(document);
            Core.Services.domModifier.updateFontSize(document);
            Core.Services.domModifier.setDisplayStyle(document);

            setTimeout(() => {

                Core.Services.domModifier.setTableWidth(document);
                Core.Services.domModifier.setTdWidth(document);
                setTimeout(() => {
                    Core.Services.domModifier.setImageSize(document);
                    Core.Services.domModifier.addCss(document, "app.css");
                    setTimeout(() => {
                        this.paginator = new Pagination.Paginator();
                        this.paginator.doPagination();
                        this.paginator.displayId(this.location.idFrom);
                        callback();
                    }, 100);
                }, 100);
            }, 100);
        }

        public openUrl(): void {
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
        }
    }
}