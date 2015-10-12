/// <reference path="core.ts" />

module Pagination {
    import Services = Core.Services;
    import DomWalkerArgs = Core.DomWalkerArgs;
    import IDomWalkerCallback = Core.IDomWalkerCallback;

    export class PageNode {
        public element: HTMLElement;
        public display: any;
        public listStyleType: any;
        public textIndent: any;

        public isElementVisible(e: HTMLElement): boolean {
            return e.style.display != 'none';
        }

        public getLiStyle(li: HTMLLIElement): void {
            this.listStyleType = li.style.listStyleType;
        }

        public getPStyle(p: HTMLParagraphElement): void {
            this.listStyleType = p.style.textIndent;
        }

        public controlBullet(li: HTMLLIElement): void {
            for (var i = 0; i < li.children.length; i++) {
                var e = li.children[i];
                if (e instanceof HTMLElement) {
                    if (this.isElementVisible(e)) {
                        li.style.listStyleType = this.listStyleType;
                    } else {
                        li.style.listStyleType = 'none';
                    }
                    return;
                }
            }
        }

        public controlParagraphSryle(p: HTMLParagraphElement): void {
            for (var i = 0; i < p.children.length; i++) {
                var e = p.children[i];
                if (e instanceof HTMLElement) {
                    if (this.isElementVisible(e)) {
                        p.style.textIndent = this.textIndent;
                    } else {
                        p.style.textIndent = 'none';
                    }
                    return;
                }
            }
        }

        public controlListItem() {
            var li = this.element;
            if (li instanceof HTMLLIElement) {
                this.controlBullet(li);
            }

        }

        public controlParagraph() {
            var p = this.element;
            if (p instanceof HTMLParagraphElement) {
                this.controlParagraphSryle(p);
            }

        }

        public show() {
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
        }

        public hide() {
            this.element.style.display = "none";
        }

        constructor(element: HTMLElement) {
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
    }

    export class Page {
        public nodes: PageNode[] = [];
        public from: number;
        public to: number;

        public include(v: number): boolean {
            if (v <= this.from)
                return false;
            if (v > this.to)
                return false;
            return true;
        }

        public cross(from: number, to: number): boolean {
            if (to < this.from)
                return false;
            if (from > this.to)
                return false;
            return true;
        }

        public show() {
            var i;
            for (i = 0; i < this.nodes.length; i++) {
                this.nodes[i].show();
            }
            for (i = 0; i < this.nodes.length; i++) {
                this.nodes[i].controlListItem();
                this.nodes[i].controlParagraph();
            }
        }

        public hide() {
            for (var i = 0; i < this.nodes.length; i++) {
                this.nodes[i].hide();
            }
        }

        public containsId(id: string): boolean {
            for (var i = 0; i < this.nodes.length; i++) {
                var elemantId: string = this.nodes[i].element.id;
                if (id == elemantId)
                    return true;
            }
            return false;
        }

        public calcLocation(url: string): Core.EpubLocation {
            var result = new Core.EpubLocation();
            result.url = url;
            result.idFrom = "";
            result.idTo = "";
            for (var i = 0; i < this.nodes.length; i++) {
                if (this.nodes[i].element instanceof HTMLSpanElement) {
                    var spanId: string = this.nodes[i].element.id;
                    if (spanId.indexOf(Core.Config.spanIdBase) == 0) {
                        if (result.idFrom != "") {
                            result.idFrom = spanId;
                        }
                        result.idTo = spanId;
                    }
                }
            }
            return result;
        }

        constructor(from: number, to: number) {
            this.from = from;
            this.to = to;
        }

    }

    export class Leaf {
        public element: HTMLElement;
        public bounds: Core.Bounds;

        constructor(element: HTMLElement) {
            this.element = element;
            this.bounds = Services.absolutBoundsDetector.getAbsolutBounds(element);
        }
    }

    export class LeafsWalker implements IDomWalkerCallback {
        public leafs: Array<Leaf> = [];
        public sorrtedLeafs: Array<Leaf> = [];

        public walkThrough(e: DomWalkerArgs): void {
            if (!e.isLeaf) return;
            var node = e.node;
            if (node instanceof HTMLElement) {
                this.leafs.push(new Leaf(node));
            }
        }

        public sort(): void {
            this.sorrtedLeafs = this.leafs.sort((l1: Leaf, l2: Leaf) => {
                var n1: number = l1.bounds.top;
                var n2: number = l2.bounds.top;
                if (n1 > n2) {
                    return 1;
                }
                if (n1 < n2) {
                    return -1;
                }
                return 0;
            });
        }
    }

    export class LeafsSpliter {
        public pageHeight: number = 500.0;
        public actualPageHeight: number;
        public spliters: number[];
        public sorrtedLeafs: Array<Leaf> = [];

        public splitRangeStart = -1;
        public splitRangeEnd = -1;

        public split(): void {
            this.spliters = [0];
            this.actualPageHeight = this.pageHeight;
            var mode = LeafsSpliterMode.DetectRangeStart;
            for (var i = 0; i < this.sorrtedLeafs.length; i++) {
                var leaf = this.sorrtedLeafs[i];
                if (mode == LeafsSpliterMode.DetectRangeStart) {
                    if (leaf.bounds.bottom > this.actualPageHeight) {
                        mode = LeafsSpliterMode.DetectRangeEnd;
                        this.splitRangeStart = i;
                    }
                }
                if (mode == LeafsSpliterMode.DetectRangeEnd) {
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
                        mode = LeafsSpliterMode.DetectRangeStart;
                    }
                }
            }
        }
    }
    export enum LeafsSpliterMode {
        DetectRangeStart,
        DetectRangeEnd
    }

    export class SplitersWalker implements IDomWalkerCallback {
        public pageHeight: number = 500.0;
        public actualPageHeight: number;
        public spliters: number[];

        public walkThrough(e: DomWalkerArgs): void {
            var bounds = Services.absolutBoundsDetector.getAbsolutBounds(e.node);
            if (bounds.bottom < this.actualPageHeight) {
                e.sckipChildren = true;
                return;
            }
            if (!e.isLeaf) return;
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
        }

        public insertHr(element: HTMLElement): HTMLHRElement {
            var hr: HTMLHRElement = document.createElement('hr');
            element.parentElement.insertBefore(hr, element);
            return hr;
        }
    }

    export enum IncludeState {
        None,
        FullInclude,
        PartialInclude
    }

    export class PagesWalker implements IDomWalkerCallback {
        public pages: Page[];
        public pageIndex: number;
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
        public walkThrough(e: DomWalkerArgs): void {
            var isLeaf = e.isLeaf;
            if (!isLeaf) return;
            var element: HTMLElement;
            var node = e.node;
            if (node instanceof HTMLElement) {
                element = node;
                var bounds = Services.absolutBoundsDetector.getLocalAbsolutBounds(element);// getAbsolutBounds(element);
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
                var p: Node = element;
                if (p instanceof HTMLElement)
                    page.nodes.push(new PageNode(p));
                while (p != document.body) {
                    if (p instanceof HTMLElement)
                        page.nodes.push(new PageNode(p));
                    p = p.parentNode;                
                }
                
            }
        }
    }

    export class Paginator {
        public splitersWalker: SplitersWalker = new SplitersWalker();
        public pagesWalker: PagesWalker = new PagesWalker();
        public pageIndex: number = -1;

        public doPagination() {
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
        }

        public getPagesCount(): number {
            return this.pagesWalker.pages.length;
        }

        public hidePage(i: number) {
            var page = this.pagesWalker.pages[i];
            page.hide();
        }

        public showPage(i: number) {
            var page = this.pagesWalker.pages[i];
            page.show();
        }

        public getCurrentPageFirstSpan() {
            if (this.pageIndex >= this.pagesWalker.pages.length)
                return 0;
            var page = this.pagesWalker.pages[this.pageIndex];
            return page.from;
        }

        public findPageIndexBySpanId(spanId: number) {
            for (var i = 0; i < this.pagesWalker.pages.length; i++) {
                var page = this.pagesWalker.pages[i];
                if ((page.from <= i) && (i <= page.to)) {
                    return i;
                }
            }
            return -1;
        }

        public calcLocation(url: string): Core.EpubLocation {
            var page = this.pagesWalker.pages[this.pageIndex];
            return page.calcLocation(url);
        }

        public displayId(id: string) {
            if (id == Core.Config.firstPageId) {
                this.displayPage(0);
            } else if (id == Core.Config.lastPageId) {
                this.displayPage(this.pagesWalker.pages.length - 1);
            }
            else
            {
                for (var i = 0; i < this.pagesWalker.pages.length; i++) {
                    var page = this.pagesWalker.pages[i];
                    if (page.containsId(id)) {
                        this.displayPage(i);
                        return;
                    }
                }
            }
        }

        public displayPage(i: number) {
            if (this.pageIndex >= 0)
                this.hidePage(this.pageIndex);
            this.pageIndex = i;
            if (this.pageIndex >= 0) {
                this.showPage(this.pageIndex);
            }
            this.normalizeHeight();
        }
        public normalizeHeight(): void {
            var pageHeight = window.innerHeight;// - Core.Config.pagePadding;
            var d = (pageHeight - document.body.offsetHeight) * 0.5;
            if (Math.abs(d) < Core.Config.pageCenterLimit) {
                document.body.style.marginTop = (pageHeight - document.body.offsetHeight) * 0.5 + "px";
            } else {
                document.body.style.marginTop = "0px";
            }
            //if (document.body.offsetHeight > pageHeight) {
            //    alert("!!!");
            //}
        }
    }
}