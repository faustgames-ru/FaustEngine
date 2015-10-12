/// <reference path="core.ts" />
declare function hyp(value: string): string;

module Common {
    import IHtmlParser = Core.IHtmlParser;
    import IDomSynchronizer = Core.IDomSynchronizer;
    import IDomModifier = Core.IDomModifier;
    import IDomWalker = Core.IDomWalker;
    import IDomWalkerCallback = Core.IDomWalkerCallback;
    import IDomWalkerFilter = Core.IDomWalkerFilter;
    import IAbsolutBoundsDetector = Core.IAbsolutBoundsDetector;
    import DomWalkerArgs = Core.DomWalkerArgs;
    import IImageLoadCallback = Core.IImageLoadCallback;

    export class DomWalker implements IDomWalker {
        args: DomWalkerArgs = new DomWalkerArgs();
        iterationsCount: number;

        public filtNodes(doc: Document, filter: IDomWalkerFilter): Node[] {
            var result: Node[] = [];
            this.filtNode(doc.body, filter, result);
            return result;
        }

        public filtNode(node: Node, filter: IDomWalkerFilter, result: Node[]): void {
            if (filter(node))
                result.push(node);
            for (var i = 0; i < node.childNodes.length; i++) {
                this.filtNode(node.childNodes[i], filter, result);
            }
        }

        public walk(doc: Document, callback: IDomWalkerCallback): number {
            this.iterationsCount = 0;
            this.args.breakWalk = false;
            this.args.sckipNext = false;
            this.walkElement(doc.body, callback);
            this.args.node = null;
            return this.iterationsCount;
        }

        public walkElement(element: HTMLElement, callback: IDomWalkerCallback): void {
            if (this.args.breakWalk) return;
            this.args.node = element;
            this.args.isLeaf = element.children.length == 0;
            this.args.sckipChildren = false;
            this.iterationsCount++;
            callback.walkThrough(this.args);
            if (this.args.sckipChildren) return;
            if (this.args.breakWalk) return;

            for (var i = 0; i < element.children.length; i++) {
                if (this.args.sckipNext) {
                    this.args.sckipNext = false;
                    continue;
                }
                var child = element.children[i];
                if (child instanceof HTMLElement) {
                    this.walkElement(child, callback);
                    if (this.args.breakWalk)
                        break;
                }
            }
        }
    }

    export class DomSynchroniser implements IDomSynchronizer {

        public getCssElements(doc: Document): HTMLLinkElement[] {
            var links: NodeListOf<HTMLLinkElement> = doc.head.getElementsByTagName("link");
            var result: HTMLLinkElement[] = [];
            for (var i = 0; i < links.length; i++) {
                if (links[i].rel == "stylesheet") {
                    result.push(links[i]);
                }
            }
            return result;
        }

        public areCssEquals(source: HTMLLinkElement[], target: HTMLLinkElement[]): boolean {
            var identical: boolean = target.length == source.length;
            var i: number;
            if (identical) {
                for (i = 0; i < target.length; i++) {
                    var targetLink = target[i];
                    var sourceLink = source[i];
                    if (targetLink.href != sourceLink.href) {
                        identical = false;
                        break;
                    }
                    if (targetLink.rel != sourceLink.rel) {
                        identical = false;
                        break;
                    }
                    if (targetLink.type != sourceLink.type) {
                        identical = false;
                        break;
                    }
                }
            }
            return identical;
        }

        public synchronizeCss(target: Document, source: Document): void {
            var targetLinks = this.getCssElements(target);
            var sourceLinks = this.getCssElements(source);
            if (this.areCssEquals(targetLinks, sourceLinks))
                return;
            var i;
            for (i = 0; i < targetLinks.length; i++) {
                target.head.removeChild(targetLinks[i]);
            }
            for (i = 0; i < sourceLinks.length; i++) {
                target.head.appendChild(sourceLinks[i]);
            }
        }

        public synchronize(target: Document, source: Document): void {
            this.synchronizeCss(target, source);
            target.body = source.body;
            
        }
    }

    export class DomModifier implements IDomModifier {

        public htmlElementsFilter(node: Node): boolean {
            return node instanceof HTMLElement;
        }

        public updateFontSize(doc: Document): void {
            var elements = Core.Services.domWalker.filtNodes(doc, this.htmlElementsFilter);
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    this.scaleFont(e);
                }
            }
        }

        public clearCss(doc: Document): void {
            var elements = Core.Services.domWalker.filtNodes(doc, this.htmlElementsFilter);
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    this.clearStyle(e);
                }
            }
        }

        public setDisplayStyle(doc: Document): void {
            var elements = Core.Services.domWalker.filtNodes(doc, this.htmlElementsFilter);
            
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    if (Core.Config.displayStyle == Core.DisplayStyle.BlackBackground) {
                        this.invertColor(e);
                    } else if (Core.Config.displayStyle == Core.DisplayStyle.WhiteBackground) {
                        this.overrideColor(e);
                    } else if (Core.Config.displayStyle == Core.DisplayStyle.Inverse) {
                        this.overrideIfDark(e);
                    }
                }
            }
            
            if (Core.Config.displayStyle == Core.DisplayStyle.BlackBackground) {
                doc.body.style.backgroundColor = 'black';
            } else if (Core.Config.displayStyle == Core.DisplayStyle.WhiteBackground) {
                doc.body.style.backgroundColor = 'while';
            } else if (Core.Config.displayStyle == Core.DisplayStyle.Inverse) {
                doc.body.style.backgroundColor = 'black';
            }
        }

        public scaleFont(e: HTMLElement): void {
            var style = window.getComputedStyle(e);
            var fontSize = parseFloat(style.fontSize);
            e.style.fontSize = Math.round(fontSize * Core.Config.fontScale) + "px";
        }

        public invertColor(e: HTMLElement): void {
            e.style.backgroundColor = "transparent";
            e.style.color = "white";
        }

        public overrideColor(e: HTMLElement): void {
            e.style.backgroundColor = "transparent";
            e.style.color = "black";
        }
        
        public overrideIfDark(e: HTMLElement): void {
            var s = window.getComputedStyle(e);
            var color = Core.ColorConverter.convertColor(s.color);
            var b = Math.max(color[0], color[1], color[2]);
            if (b < 1.0)
                e.style.color = "white";
            e.style.backgroundColor = "transparent";
        }

        public updateImageSources(doc: Document, baseUrl: string, callback: IImageLoadCallback): boolean {
            var images: NodeListOf<HTMLImageElement> = doc.getElementsByTagName('img');
            var count = 0;
            var total = images.length;
            var i;
            for (i = 0; i < images.length; i++) {
                images[i].onload = () => {
                    count++;
                    if (count == total) {
                        callback();
                    }
                }
            }

            for (i = 0; i < images.length; i++) {
                var src = baseUrl + images[i].getAttribute("src");
                images[i].src = src;
            }
            return images.length > 0;
        }

        public updateCssRefs(doc: Document, baseUrl: string): void {
            var links: NodeListOf<HTMLLinkElement> = doc.getElementsByTagName('link');
            for (var i = 0; i < links.length; i++) {
                var href = baseUrl + links[i].getAttribute("href");
                links[i].href = href;
            }
        }

        public textsFilter(node: Node): boolean {
            return node.nodeType == Node.TEXT_NODE;
        }

        public createSpansFormWords(doc: Document): void {
            var texts = Core.Services.domWalker.filtNodes(doc, this.textsFilter);
            var id = 0;
            for (var i = 0; i < texts.length; i++) {
                var textNode = texts[i];
                var value = textNode.nodeValue;
                var trim = value.trim();
                if (trim == "") continue;
                var words = value.split(" ");
                var parent = textNode.parentNode;
                for (var j = 0; j < words.length; j++) {
                    var word = Core.Config.hyphynate ? hyp(words[j]) : words[j];
                    var span = doc.createElement("span");
                    span.onmouseup = snapSelectionToWord;
                    span.id = Core.Config.spanIdBase + id;
                    if (j == (words.length - 1)) {
                        span.appendChild(doc.createTextNode(word));
                    } else {
                        span.appendChild(doc.createTextNode(word + " "));
                    }
                    parent.insertBefore(span, textNode);
                }
                parent.removeChild(textNode);
            }
        }

        public setTableWidth(doc: Document): void {
            var tables: NodeListOf<HTMLTableElement> = doc.getElementsByTagName('table');
            for (var i = 0; i < tables.length; i++) {
                var table = tables[i];

                var w = window.innerWidth * Core.Config.tablesWidthScale;
                if (table.offsetWidth > w) {
                    table.style.width = w + "px";
                }

                if (tables[i].offsetWidth < Core.Config.tablesMinWidth)
                    tables[i].style.width = Core.Config.tablesMinWidth + "px";
                
                if (table.offsetWidth > window.innerWidth) {
                    // todo: replace table with link 
                    table.style.display = 'none';
                    //table.style.width = window.innerWidth + "px";
                }
            }
        }

        public clearStyle(e: HTMLElement): void {
            e.style.marginTop = e.style.marginBottom = "0px";
            e.style.paddingTop = e.style.paddingBottom = "0px";
        }

        public clearStyles(doc: Document, elementName: string): void {
            var elements = doc.getElementsByTagName(elementName);
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    this.clearStyle(e);
                }
            }
        }

        public setTdWidth(doc: Document): void {
            var tds: NodeListOf<HTMLTableDataCellElement> = doc.getElementsByTagName('td');
            var i;
            for (i = 0; i < tds.length; i++) {
                if (Core.Config.clearTablesMargingsAndPaddings) {
                    this.clearStyle(tds[i]);
                }
                tds[i].style.width = tds[i].offsetWidth + "px";
            }
            var ths: NodeListOf<HTMLTableDataCellElement> = doc.getElementsByTagName('th');
            for (i = 0; i < ths.length; i++) {
                if (Core.Config.clearTablesMargingsAndPaddings) {
                    this.clearStyle(tds[i]);
                }
                ths[i].style.width = ths[i].offsetWidth + "px";
            }

        }

        public setImageSize(doc: Document): void {
            var images: NodeListOf<HTMLImageElement> = doc.getElementsByTagName('img');
            var maxW = window.innerWidth * Core.Config.imagesMaxWScale;
            var maxH = (window.innerHeight * Core.Config.pageHeightScale - Core.Config.pagePadding) * Core.Config.imagesMaxHScale;
            for (var i = 0; i < images.length; i++) {
                var image = images[i];
                var style = window.getComputedStyle(image);
                var w: number = parseFloat(style.width);
                var h: number = parseFloat(style.height);
                var aspect = h / w;
                if (w > maxW) {
                    w = maxW;
                    h = maxW * aspect;
                    image.style.width = Math.round(w) + "px";
                    image.style.height = Math.round(h) + "px";
                }
                if (h > maxH) {
                    image.style.width = Math.round(maxH / aspect) + "px";
                    image.style.height = Math.round(maxH) + "px";
                }
            }
        }


        public addCss(doc: Document, href: string): void {
            var css = doc.createElement("link");
            css.href = href;
            css.rel = "stylesheet";
            css.type = "text/css";
            doc.head.appendChild(css);
        }

        public justifyParagraphs(doc: Document): void {
            var ps: NodeListOf<HTMLParagraphElement> = doc.body.getElementsByTagName('p');
            for (var i = 0; i < ps.length; i++) {
                var style = window.getComputedStyle(ps[i]);
                if ((style.textAlign == "left") || (style.textAlign == "") || (style.textAlign == null)) {
                    ps[i].style.textAlign = "justify";
                }
            }
        }
    }

    export class DefaultDomParser implements IHtmlParser {
        public parse(text: string, mimeType: string): Document {
            var parser = new DOMParser();
            var doc = parser.parseFromString(text, mimeType);
            return doc;
        }
    }


    export class AbsolutBoundsDetector implements IAbsolutBoundsDetector {
        public expandBounds(e: HTMLElement, b: Core.Bounds) {
            var style = window.getComputedStyle(e);
            var top = parseFloat(style.marginTop) + parseFloat(style.paddingTop);
            var bottom = parseFloat(style.marginBottom) + parseFloat(style.paddingBottom);
            b.top -= top;
            b.bottom += bottom;
        }

        public getLocalAbsolutBounds(element: HTMLElement): Core.Bounds {
            var b = new Core.Bounds();
            var r = element.getBoundingClientRect();
            b.left = r.left;
            b.right = r.right;
            b.top = r.top;
            b.bottom = r.bottom;
            return b;
        }

        public getAbsolutBounds(element: HTMLElement): Core.Bounds {
            var b = new Core.Bounds();
            var r = element.getBoundingClientRect();
            b.left = r.left;
            b.right = r.right;
            b.top= r.top;
            b.bottom = r.bottom;
            var p = element.parentNode;
            while (p != null) {
                if (p instanceof HTMLElement) {
                    this.expandBounds(p, b);
                }
                p = p.parentNode;
            }
            return b;
        }

    }
    
    function snapSelectionToWord() {
        var sel: any = window.getSelection();

        // Check for existence of window.getSelection() and that it has a
        // modify() method. IE 9 has both selection APIs but no modify() method.
        if (window.getSelection && (sel = sel.modify)) {
            sel = window.getSelection();
            if (!sel.isCollapsed) {

                // Detect if selection is backwards
                var range = document.createRange();
                range.setStart(sel.anchorNode, sel.anchorOffset);
                range.setEnd(sel.focusNode, sel.focusOffset);
                var backwards = range.collapsed;
                range.detach();

                // modify() works on the focus of the selection
                var endNode = sel.focusNode, endOffset = sel.focusOffset;
                sel.collapse(sel.anchorNode, sel.anchorOffset);
                if (backwards) {
                    sel.modify("move", "forward", "word");
                    sel.extend(endNode, endOffset);
                    sel.modify("extend", "backward", "word");

                } else {                                                                                                
                    sel.modify("move", "backward", "word");
                    sel.extend(endNode, endOffset);
                    sel.modify("extend", "forward", "word");
                }
            }
        } else if ((sel = document.selection) && sel.type != "Control") {
            var textRange = sel.createRange();
            if (textRange.text) {
                textRange.expand("word");
                // Move the end back to not include the word's trailing space(s),
                // if necessary
                while (/\s$/.test(textRange.text)) {
                    textRange.moveEnd("character", -1);
                }
                textRange.select();
            }
        }
    }

} 