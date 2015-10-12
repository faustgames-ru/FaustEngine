/// <reference path="core.ts" />
var Common;
(function (Common) {
    var DomWalkerArgs = Core.DomWalkerArgs;
    var DomWalker = (function () {
        function DomWalker() {
            this.args = new DomWalkerArgs();
        }
        DomWalker.prototype.filtNodes = function (doc, filter) {
            var result = [];
            this.filtNode(doc.body, filter, result);
            return result;
        };
        DomWalker.prototype.filtNode = function (node, filter, result) {
            if (filter(node))
                result.push(node);
            for (var i = 0; i < node.childNodes.length; i++) {
                this.filtNode(node.childNodes[i], filter, result);
            }
        };
        DomWalker.prototype.walk = function (doc, callback) {
            this.iterationsCount = 0;
            this.args.breakWalk = false;
            this.args.sckipNext = false;
            this.walkElement(doc.body, callback);
            this.args.node = null;
            return this.iterationsCount;
        };
        DomWalker.prototype.walkElement = function (element, callback) {
            if (this.args.breakWalk)
                return;
            this.args.node = element;
            this.args.isLeaf = element.children.length == 0;
            this.args.sckipChildren = false;
            this.iterationsCount++;
            callback.walkThrough(this.args);
            if (this.args.sckipChildren)
                return;
            if (this.args.breakWalk)
                return;
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
        };
        return DomWalker;
    })();
    Common.DomWalker = DomWalker;
    var DomSynchroniser = (function () {
        function DomSynchroniser() {
        }
        DomSynchroniser.prototype.getCssElements = function (doc) {
            var links = doc.head.getElementsByTagName("link");
            var result = [];
            for (var i = 0; i < links.length; i++) {
                if (links[i].rel == "stylesheet") {
                    result.push(links[i]);
                }
            }
            return result;
        };
        DomSynchroniser.prototype.areCssEquals = function (source, target) {
            var identical = target.length == source.length;
            var i;
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
        };
        DomSynchroniser.prototype.synchronizeCss = function (target, source) {
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
        };
        DomSynchroniser.prototype.synchronize = function (target, source) {
            this.synchronizeCss(target, source);
            target.body = source.body;
        };
        return DomSynchroniser;
    })();
    Common.DomSynchroniser = DomSynchroniser;
    var DomModifier = (function () {
        function DomModifier() {
        }
        DomModifier.prototype.htmlElementsFilter = function (node) {
            return node instanceof HTMLElement;
        };
        DomModifier.prototype.updateFontSize = function (doc) {
            var elements = Core.Services.domWalker.filtNodes(doc, this.htmlElementsFilter);
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    this.scaleFont(e);
                }
            }
        };
        DomModifier.prototype.clearCss = function (doc) {
            var elements = Core.Services.domWalker.filtNodes(doc, this.htmlElementsFilter);
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    this.clearStyle(e);
                }
            }
        };
        DomModifier.prototype.setDisplayStyle = function (doc) {
            var elements = Core.Services.domWalker.filtNodes(doc, this.htmlElementsFilter);
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    if (Core.Config.displayStyle == 1 /* BlackBackground */) {
                        this.invertColor(e);
                    }
                    else if (Core.Config.displayStyle == 2 /* WhiteBackground */) {
                        this.overrideColor(e);
                    }
                    else if (Core.Config.displayStyle == 3 /* Inverse */) {
                        this.overrideIfDark(e);
                    }
                }
            }
            if (Core.Config.displayStyle == 1 /* BlackBackground */) {
                doc.body.style.backgroundColor = 'black';
            }
            else if (Core.Config.displayStyle == 2 /* WhiteBackground */) {
                doc.body.style.backgroundColor = 'while';
            }
            else if (Core.Config.displayStyle == 3 /* Inverse */) {
                doc.body.style.backgroundColor = 'black';
            }
        };
        DomModifier.prototype.scaleFont = function (e) {
            var style = window.getComputedStyle(e);
            var fontSize = parseFloat(style.fontSize);
            e.style.fontSize = Math.round(fontSize * Core.Config.fontScale) + "px";
        };
        DomModifier.prototype.invertColor = function (e) {
            e.style.backgroundColor = "transparent";
            e.style.color = "white";
        };
        DomModifier.prototype.overrideColor = function (e) {
            e.style.backgroundColor = "transparent";
            e.style.color = "black";
        };
        DomModifier.prototype.overrideIfDark = function (e) {
            var s = window.getComputedStyle(e);
            var color = Core.ColorConverter.convertColor(s.color);
            var b = Math.max(color[0], color[1], color[2]);
            if (b < 1.0)
                e.style.color = "white";
            e.style.backgroundColor = "transparent";
        };
        DomModifier.prototype.updateImageSources = function (doc, baseUrl, callback) {
            var images = doc.getElementsByTagName('img');
            var count = 0;
            var total = images.length;
            var i;
            for (i = 0; i < images.length; i++) {
                images[i].onload = function () {
                    count++;
                    if (count == total) {
                        callback();
                    }
                };
            }
            for (i = 0; i < images.length; i++) {
                var src = baseUrl + images[i].getAttribute("src");
                images[i].src = src;
            }
            return images.length > 0;
        };
        DomModifier.prototype.updateCssRefs = function (doc, baseUrl) {
            var links = doc.getElementsByTagName('link');
            for (var i = 0; i < links.length; i++) {
                var href = baseUrl + links[i].getAttribute("href");
                links[i].href = href;
            }
        };
        DomModifier.prototype.textsFilter = function (node) {
            return node.nodeType == Node.TEXT_NODE;
        };
        DomModifier.prototype.createSpansFormWords = function (doc) {
            var texts = Core.Services.domWalker.filtNodes(doc, this.textsFilter);
            var id = 0;
            for (var i = 0; i < texts.length; i++) {
                var textNode = texts[i];
                var value = textNode.nodeValue;
                var trim = value.trim();
                if (trim == "")
                    continue;
                var words = value.split(" ");
                var parent = textNode.parentNode;
                for (var j = 0; j < words.length; j++) {
                    var word = Core.Config.hyphynate ? hyp(words[j]) : words[j];
                    var span = doc.createElement("span");
                    span.onmouseup = snapSelectionToWord;
                    span.id = Core.Config.spanIdBase + id;
                    if (j == (words.length - 1)) {
                        span.appendChild(doc.createTextNode(word));
                    }
                    else {
                        span.appendChild(doc.createTextNode(word + " "));
                    }
                    parent.insertBefore(span, textNode);
                }
                parent.removeChild(textNode);
            }
        };
        DomModifier.prototype.setTableWidth = function (doc) {
            var tables = doc.getElementsByTagName('table');
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
                }
            }
        };
        DomModifier.prototype.clearStyle = function (e) {
            e.style.marginTop = e.style.marginBottom = "0px";
            e.style.paddingTop = e.style.paddingBottom = "0px";
        };
        DomModifier.prototype.clearStyles = function (doc, elementName) {
            var elements = doc.getElementsByTagName(elementName);
            for (var i = 0; i < elements.length; i++) {
                var e = elements[i];
                if (e instanceof HTMLElement) {
                    this.clearStyle(e);
                }
            }
        };
        DomModifier.prototype.setTdWidth = function (doc) {
            var tds = doc.getElementsByTagName('td');
            var i;
            for (i = 0; i < tds.length; i++) {
                if (Core.Config.clearTablesMargingsAndPaddings) {
                    this.clearStyle(tds[i]);
                }
                tds[i].style.width = tds[i].offsetWidth + "px";
            }
            var ths = doc.getElementsByTagName('th');
            for (i = 0; i < ths.length; i++) {
                if (Core.Config.clearTablesMargingsAndPaddings) {
                    this.clearStyle(tds[i]);
                }
                ths[i].style.width = ths[i].offsetWidth + "px";
            }
        };
        DomModifier.prototype.setImageSize = function (doc) {
            var images = doc.getElementsByTagName('img');
            var maxW = window.innerWidth * Core.Config.imagesMaxWScale;
            var maxH = (window.innerHeight * Core.Config.pageHeightScale - Core.Config.pagePadding) * Core.Config.imagesMaxHScale;
            for (var i = 0; i < images.length; i++) {
                var image = images[i];
                var style = window.getComputedStyle(image);
                var w = parseFloat(style.width);
                var h = parseFloat(style.height);
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
        };
        DomModifier.prototype.addCss = function (doc, href) {
            var css = doc.createElement("link");
            css.href = href;
            css.rel = "stylesheet";
            css.type = "text/css";
            doc.head.appendChild(css);
        };
        DomModifier.prototype.justifyParagraphs = function (doc) {
            var ps = doc.body.getElementsByTagName('p');
            for (var i = 0; i < ps.length; i++) {
                var style = window.getComputedStyle(ps[i]);
                if ((style.textAlign == "left") || (style.textAlign == "") || (style.textAlign == null)) {
                    ps[i].style.textAlign = "justify";
                }
            }
        };
        return DomModifier;
    })();
    Common.DomModifier = DomModifier;
    var DefaultDomParser = (function () {
        function DefaultDomParser() {
        }
        DefaultDomParser.prototype.parse = function (text, mimeType) {
            var parser = new DOMParser();
            var doc = parser.parseFromString(text, mimeType);
            return doc;
        };
        return DefaultDomParser;
    })();
    Common.DefaultDomParser = DefaultDomParser;
    var AbsolutBoundsDetector = (function () {
        function AbsolutBoundsDetector() {
        }
        AbsolutBoundsDetector.prototype.expandBounds = function (e, b) {
            var style = window.getComputedStyle(e);
            var top = parseFloat(style.marginTop) + parseFloat(style.paddingTop);
            var bottom = parseFloat(style.marginBottom) + parseFloat(style.paddingBottom);
            b.top -= top;
            b.bottom += bottom;
        };
        AbsolutBoundsDetector.prototype.getLocalAbsolutBounds = function (element) {
            var b = new Core.Bounds();
            var r = element.getBoundingClientRect();
            b.left = r.left;
            b.right = r.right;
            b.top = r.top;
            b.bottom = r.bottom;
            return b;
        };
        AbsolutBoundsDetector.prototype.getAbsolutBounds = function (element) {
            var b = new Core.Bounds();
            var r = element.getBoundingClientRect();
            b.left = r.left;
            b.right = r.right;
            b.top = r.top;
            b.bottom = r.bottom;
            var p = element.parentNode;
            while (p != null) {
                if (p instanceof HTMLElement) {
                    this.expandBounds(p, b);
                }
                p = p.parentNode;
            }
            return b;
        };
        return AbsolutBoundsDetector;
    })();
    Common.AbsolutBoundsDetector = AbsolutBoundsDetector;
    function snapSelectionToWord() {
        var sel = window.getSelection();
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
                }
                else {
                    sel.modify("move", "backward", "word");
                    sel.extend(endNode, endOffset);
                    sel.modify("extend", "forward", "word");
                }
            }
        }
        else if ((sel = document.selection) && sel.type != "Control") {
            var textRange = sel.createRange();
            if (textRange.text) {
                textRange.expand("word");
                while (/\s$/.test(textRange.text)) {
                    textRange.moveEnd("character", -1);
                }
                textRange.select();
            }
        }
    }
})(Common || (Common = {}));
//# sourceMappingURL=common.js.map