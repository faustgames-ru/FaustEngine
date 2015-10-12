/// <reference path="core.ts" />
var Pagination;
(function (Pagination) {
    var Services = Core.Services;
    var PageNode = (function () {
        function PageNode(element) {
            this.element = element;
            this.display = element.style.display;
            var li = this.element;
            if (li instanceof HTMLLIElement) {
                this.getLiStyle(li);
            }
            var p = this.element;
            if (p instanceof HTMLParagraphElement) {
                this.getPStyle(p);
            }
        }
        PageNode.prototype.isElementVisible = function (e) {
            return e.style.display != 'none';
        };
        PageNode.prototype.getLiStyle = function (li) {
            this.listStyleType = li.style.listStyleType;
        };
        PageNode.prototype.getPStyle = function (p) {
            this.listStyleType = p.style.textIndent;
        };
        PageNode.prototype.controlBullet = function (li) {
            for (var i = 0; i < li.children.length; i++) {
                var e = li.children[i];
                if (e instanceof HTMLElement) {
                    if (this.isElementVisible(e)) {
                        li.style.listStyleType = this.listStyleType;
                    }
                    else {
                        li.style.listStyleType = 'none';
                    }
                    return;
                }
            }
        };
        PageNode.prototype.controlParagraphSryle = function (p) {
            for (var i = 0; i < p.children.length; i++) {
                var e = p.children[i];
                if (e instanceof HTMLElement) {
                    if (this.isElementVisible(e)) {
                        p.style.textIndent = this.textIndent;
                    }
                    else {
                        p.style.textIndent = 'none';
                    }
                    return;
                }
            }
        };
        PageNode.prototype.controlListItem = function () {
            var li = this.element;
            if (li instanceof HTMLLIElement) {
                this.controlBullet(li);
            }
        };
        PageNode.prototype.controlParagraph = function () {
            var p = this.element;
            if (p instanceof HTMLParagraphElement) {
                this.controlParagraphSryle(p);
            }
        };
        PageNode.prototype.show = function () {
            this.element.style.display = this.display;
            if (this.element instanceof HTMLTableRowElement) {
                var tds = this.element.getElementsByTagName('td');
                var i;
                for (i = 0; i < tds.length; i++) {
                    tds[i].style.display = this.display;
                }
                var ths = this.element.getElementsByTagName('th');
                for (i = 0; i < ths.length; i++) {
                    ths[i].style.display = this.display;
                }
            }
        };
        PageNode.prototype.hide = function () {
            this.element.style.display = "none";
        };
        return PageNode;
    })();
    Pagination.PageNode = PageNode;
    var Page = (function () {
        function Page(from, to) {
            this.nodes = [];
            this.from = from;
            this.to = to;
        }
        Page.prototype.include = function (v) {
            if (v <= this.from)
                return false;
            if (v > this.to)
                return false;
            return true;
        };
        Page.prototype.cross = function (from, to) {
            if (to < this.from)
                return false;
            if (from > this.to)
                return false;
            return true;
        };
        Page.prototype.show = function () {
            var i;
            for (i = 0; i < this.nodes.length; i++) {
                this.nodes[i].show();
            }
            for (i = 0; i < this.nodes.length; i++) {
                this.nodes[i].controlListItem();
                this.nodes[i].controlParagraph();
            }
        };
        Page.prototype.hide = function () {
            for (var i = 0; i < this.nodes.length; i++) {
                this.nodes[i].hide();
            }
        };
        Page.prototype.containsId = function (id) {
            for (var i = 0; i < this.nodes.length; i++) {
                var elemantId = this.nodes[i].element.id;
                if (id == elemantId)
                    return true;
            }
            return false;
        };
        Page.prototype.calcLocation = function (url) {
            var result = new Core.EpubLocation();
            result.url = url;
            result.idFrom = "";
            result.idTo = "";
            for (var i = 0; i < this.nodes.length; i++) {
                if (this.nodes[i].element instanceof HTMLSpanElement) {
                    var spanId = this.nodes[i].element.id;
                    if (spanId.indexOf(Core.Config.spanIdBase) == 0) {
                        if (result.idFrom != "") {
                            result.idFrom = spanId;
                        }
                        result.idTo = spanId;
                    }
                }
            }
            return result;
        };
        return Page;
    })();
    Pagination.Page = Page;
    var Leaf = (function () {
        function Leaf(element) {
            this.element = element;
            this.bounds = Services.absolutBoundsDetector.getAbsolutBounds(element);
        }
        return Leaf;
    })();
    Pagination.Leaf = Leaf;
    var LeafsWalker = (function () {
        function LeafsWalker() {
            this.leafs = [];
            this.sorrtedLeafs = [];
        }
        LeafsWalker.prototype.walkThrough = function (e) {
            if (!e.isLeaf)
                return;
            var node = e.node;
            if (node instanceof HTMLElement) {
                this.leafs.push(new Leaf(node));
            }
        };
        LeafsWalker.prototype.sort = function () {
            this.sorrtedLeafs = this.leafs.sort(function (l1, l2) {
                var n1 = l1.bounds.top;
                var n2 = l2.bounds.top;
                if (n1 > n2) {
                    return 1;
                }
                if (n1 < n2) {
                    return -1;
                }
                return 0;
            });
        };
        return LeafsWalker;
    })();
    Pagination.LeafsWalker = LeafsWalker;
    var LeafsSpliter = (function () {
        function LeafsSpliter() {
            this.pageHeight = 500.0;
            this.sorrtedLeafs = [];
            this.splitRangeStart = -1;
            this.splitRangeEnd = -1;
        }
        LeafsSpliter.prototype.split = function () {
            this.spliters = [0];
            this.actualPageHeight = this.pageHeight;
            var mode = 0 /* DetectRangeStart */;
            for (var i = 0; i < this.sorrtedLeafs.length; i++) {
                var leaf = this.sorrtedLeafs[i];
                if (mode == 0 /* DetectRangeStart */) {
                    if (leaf.bounds.bottom > this.actualPageHeight) {
                        mode = 1 /* DetectRangeEnd */;
                        this.splitRangeStart = i;
                    }
                }
                if (mode == 1 /* DetectRangeEnd */) {
                    if (leaf.bounds.top > this.actualPageHeight) {
                        this.splitRangeEnd = i;
                        var h = this.actualPageHeight;
                        for (var j = this.splitRangeStart; j < this.splitRangeEnd; j++) {
                            var crossLeaf = this.sorrtedLeafs[j];
                            /// todo: parent margins calcuations;
                            if (h > crossLeaf.bounds.top)
                                h = crossLeaf.bounds.top;
                        }
                        this.spliters.push(h);
                        this.actualPageHeight = h + this.pageHeight;
                        i = this.splitRangeStart;
                        mode = 0 /* DetectRangeStart */;
                    }
                }
            }
        };
        return LeafsSpliter;
    })();
    Pagination.LeafsSpliter = LeafsSpliter;
    (function (LeafsSpliterMode) {
        LeafsSpliterMode[LeafsSpliterMode["DetectRangeStart"] = 0] = "DetectRangeStart";
        LeafsSpliterMode[LeafsSpliterMode["DetectRangeEnd"] = 1] = "DetectRangeEnd";
    })(Pagination.LeafsSpliterMode || (Pagination.LeafsSpliterMode = {}));
    var LeafsSpliterMode = Pagination.LeafsSpliterMode;
    var SplitersWalker = (function () {
        function SplitersWalker() {
            this.pageHeight = 500.0;
        }
        SplitersWalker.prototype.walkThrough = function (e) {
            var bounds = Services.absolutBoundsDetector.getAbsolutBounds(e.node);
            if (bounds.bottom < this.actualPageHeight) {
                e.sckipChildren = true;
                return;
            }
            if (!e.isLeaf)
                return;
            e.sckipChildren = true;
            if (Core.Config.displayDebugHr) {
                var node = e.node;
                if (node instanceof HTMLElement) {
                    var hr = this.insertHr(node);
                    e.sckipNext = true;
                    bounds = Services.absolutBoundsDetector.getAbsolutBounds(hr);
                }
            }
            this.spliters.push(bounds.top);
            this.actualPageHeight = bounds.top + this.pageHeight;
        };
        SplitersWalker.prototype.insertHr = function (element) {
            var hr = document.createElement('hr');
            element.parentElement.insertBefore(hr, element);
            return hr;
        };
        return SplitersWalker;
    })();
    Pagination.SplitersWalker = SplitersWalker;
    (function (IncludeState) {
        IncludeState[IncludeState["None"] = 0] = "None";
        IncludeState[IncludeState["FullInclude"] = 1] = "FullInclude";
        IncludeState[IncludeState["PartialInclude"] = 2] = "PartialInclude";
    })(Pagination.IncludeState || (Pagination.IncludeState = {}));
    var IncludeState = Pagination.IncludeState;
    var PagesWalker = (function () {
        function PagesWalker() {
        }
        /*
        public insertIntoPage(element: HTMLElement, isLeaf: boolean): IncludeState {
            var page = this.pages[this.pageIndex];
            var bounds = Services.absolutBoundsDetector.getAbsolutBounds(element);

            var result: IncludeState = IncludeState.None;
            if (page.include(bounds.bottom) && page.include(bounds.top)) {
                result = IncludeState.FullInclude;
            } else if (page.cross(bounds.top, bounds.bottom)) {
                if (!isLeaf) {
                    result = IncludeState.PartialInclude;
                }
            }
            if (result != IncludeState.None)
                page.nodes.push(new PageNode(element));
            return result;
        }
        */
        // todo: optimize spliting
        PagesWalker.prototype.walkThrough = function (e) {
            var isLeaf = e.isLeaf;
            if (!isLeaf)
                return;
            var element;
            var node = e.node;
            if (node instanceof HTMLElement) {
                element = node;
                var bounds = Services.absolutBoundsDetector.getLocalAbsolutBounds(element); // getAbsolutBounds(element);
                var page = this.pages[this.pageIndex];
                if (!page.include(bounds.bottom)) {
                    for (var i = 0; i < this.pages.length; i++) {
                        if (i == this.pageIndex)
                            continue;
                        page = this.pages[i];
                        if (page.include(bounds.bottom)) {
                            break;
                        }
                    }
                }
                var p = element;
                if (p instanceof HTMLElement)
                    page.nodes.push(new PageNode(p));
                while (p != document.body) {
                    if (p instanceof HTMLElement)
                        page.nodes.push(new PageNode(p));
                    p = p.parentNode;
                }
            }
        };
        return PagesWalker;
    })();
    Pagination.PagesWalker = PagesWalker;
    var Paginator = (function () {
        function Paginator() {
            this.splitersWalker = new SplitersWalker();
            this.pagesWalker = new PagesWalker();
            this.pageIndex = -1;
        }
        Paginator.prototype.doPagination = function () {
            var pageHeight = window.innerHeight * Core.Config.pageHeightScale - Core.Config.pagePadding;
            /*
            this.leafsWalker.leafs = [];
            Services.domWalker.walk(document, this.leafsWalker);
            this.leafsWalker.sort();

            this.leafsSpliter.pageHeight = pageHeight;
            this.leafsSpliter.sorrtedLeafs = this.leafsWalker.sorrtedLeafs;
            this.leafsSpliter.split();
            var spliters = this.leafsSpliter.spliters;
            */
            this.splitersWalker.pageHeight = pageHeight;
            this.splitersWalker.spliters = [0];
            this.splitersWalker.actualPageHeight = this.splitersWalker.pageHeight;
            Services.domWalker.walk(document, this.splitersWalker);
            var spliters = this.splitersWalker.spliters;
            this.pagesWalker.pages = [];
            for (var i = 1; i < spliters.length; i++) {
                var from = spliters[i - 1];
                var to = spliters[i];
                this.pagesWalker.pages.push(new Page(from, to));
            }
            var last = spliters[spliters.length - 1];
            this.pagesWalker.pages.push(new Page(last, last + pageHeight));
            this.pagesWalker.pageIndex = 0;
            Services.domWalker.walk(document, this.pagesWalker);
            for (var j = 0; j < this.getPagesCount(); j++) {
                this.hidePage(j);
            }
            this.pageIndex = -1;
        };
        Paginator.prototype.getPagesCount = function () {
            return this.pagesWalker.pages.length;
        };
        Paginator.prototype.hidePage = function (i) {
            var page = this.pagesWalker.pages[i];
            page.hide();
        };
        Paginator.prototype.showPage = function (i) {
            var page = this.pagesWalker.pages[i];
            page.show();
        };
        Paginator.prototype.getCurrentPageFirstSpan = function () {
            if (this.pageIndex >= this.pagesWalker.pages.length)
                return 0;
            var page = this.pagesWalker.pages[this.pageIndex];
            return page.from;
        };
        Paginator.prototype.findPageIndexBySpanId = function (spanId) {
            for (var i = 0; i < this.pagesWalker.pages.length; i++) {
                var page = this.pagesWalker.pages[i];
                if ((page.from <= i) && (i <= page.to)) {
                    return i;
                }
            }
            return -1;
        };
        Paginator.prototype.calcLocation = function (url) {
            var page = this.pagesWalker.pages[this.pageIndex];
            return page.calcLocation(url);
        };
        Paginator.prototype.displayId = function (id) {
            if (id == Core.Config.firstPageId) {
                this.displayPage(0);
            }
            else if (id == Core.Config.lastPageId) {
                this.displayPage(this.pagesWalker.pages.length - 1);
            }
            else {
                for (var i = 0; i < this.pagesWalker.pages.length; i++) {
                    var page = this.pagesWalker.pages[i];
                    if (page.containsId(id)) {
                        this.displayPage(i);
                        return;
                    }
                }
            }
        };
        Paginator.prototype.displayPage = function (i) {
            if (this.pageIndex >= 0)
                this.hidePage(this.pageIndex);
            this.pageIndex = i;
            if (this.pageIndex >= 0) {
                this.showPage(this.pageIndex);
            }
            this.normalizeHeight();
        };
        Paginator.prototype.normalizeHeight = function () {
            var pageHeight = window.innerHeight; // - Core.Config.pagePadding;
            var d = (pageHeight - document.body.offsetHeight) * 0.5;
            if (Math.abs(d) < Core.Config.pageCenterLimit) {
                document.body.style.marginTop = (pageHeight - document.body.offsetHeight) * 0.5 + "px";
            }
            else {
                document.body.style.marginTop = "0px";
            }
            //if (document.body.offsetHeight > pageHeight) {
            //    alert("!!!");
            //}
        };
        return Paginator;
    })();
    Pagination.Paginator = Paginator;
})(Pagination || (Pagination = {}));
//# sourceMappingURL=pagination.js.map