var Core;
(function (Core) {
    (function (TableReplaceMode) {
        TableReplaceMode[TableReplaceMode["ReplaceNone"] = 0] = "ReplaceNone";
        TableReplaceMode[TableReplaceMode["ReplaceAll"] = 1] = "ReplaceAll";
        TableReplaceMode[TableReplaceMode["ReplaceLarge"] = 2] = "ReplaceLarge";
    })(Core.TableReplaceMode || (Core.TableReplaceMode = {}));
    var TableReplaceMode = Core.TableReplaceMode;
    (function (DisplayStyle) {
        DisplayStyle[DisplayStyle["ByCss"] = 0] = "ByCss";
        DisplayStyle[DisplayStyle["BlackBackground"] = 1] = "BlackBackground";
        DisplayStyle[DisplayStyle["WhiteBackground"] = 2] = "WhiteBackground";
        DisplayStyle[DisplayStyle["Inverse"] = 3] = "Inverse";
    })(Core.DisplayStyle || (Core.DisplayStyle = {}));
    var DisplayStyle = Core.DisplayStyle;
    var EpubLocation = (function () {
        function EpubLocation() {
        }
        EpubLocation.prototype.createClone = function () {
            var clone = new EpubLocation();
            clone.url = this.url;
            clone.idFrom = this.idFrom;
            clone.idTo = this.idTo;
            return clone;
        };
        return EpubLocation;
    })();
    Core.EpubLocation = EpubLocation;
    var Config = (function () {
        function Config() {
        }
        Config.firstPageId = "_firstPageId_";
        Config.lastPageId = "_lastPageId_";
        Config.spanIdBase = "_span_";
        Config.pageCenterLimit = 50;
        Config.tablesWidthScale = 0.95;
        Config.tablesMinWidth = 300;
        Config.pagePadding = 10;
        Config.pageHeightScale = 1.0;
        Config.imagesMaxWScale = 0.95;
        Config.imagesMaxHScale = 0.95;
        Config.fontScale = 1.05;
        Config.hyphynate = true;
        Config.tableReplaceMode = 2 /* ReplaceLarge */;
        Config.justifyParagraphs = false;
        Config.clearTablesMargingsAndPaddings = true;
        Config.displayStyle = 1 /* BlackBackground */;
        Config.displayDebugHr = false;
        return Config;
    })();
    Core.Config = Config;
    var EpubFiles = (function () {
        function EpubFiles() {
        }
        EpubFiles.toc = "toc.ncx";
        return EpubFiles;
    })();
    Core.EpubFiles = EpubFiles;
    (function (LoadingStatus) {
        LoadingStatus[LoadingStatus["Ok"] = 0] = "Ok";
        LoadingStatus[LoadingStatus["Failed"] = 1] = "Failed";
    })(Core.LoadingStatus || (Core.LoadingStatus = {}));
    var LoadingStatus = Core.LoadingStatus;
    var DomWalkerArgs = (function () {
        function DomWalkerArgs() {
        }
        return DomWalkerArgs;
    })();
    Core.DomWalkerArgs = DomWalkerArgs;
    var Bounds = (function () {
        function Bounds() {
        }
        return Bounds;
    })();
    Core.Bounds = Bounds;
    var Path = (function () {
        function Path() {
        }
        Path.getDirectory = function (path) {
            var items = path.split("/");
            var result = "";
            for (var i = 0; i < (items.length - 1); i++) {
                result += items[i] + "/";
            }
            return result;
        };
        return Path;
    })();
    Core.Path = Path;
    var Services = (function () {
        function Services() {
        }
        return Services;
    })();
    Core.Services = Services;
    var ColorConverter = (function () {
        function ColorConverter() {
        }
        ColorConverter.convertColor = function (color) {
            var rgbColors = [0, 0, 0];
            ///////////////////////////////////
            // Handle rgb(redValue, greenValue, blueValue) format
            //////////////////////////////////
            var rgb;
            if (color[0] == 'r') {
                // Find the index of the redValue.  Using subscring function to 
                // get rid off "rgb(" and ")" part.  
                // The indexOf function returns the index of the "(" and ")" which we 
                // then use to get inner content.  
                color = color.substring(color.indexOf('(') + 1, color.indexOf(')'));
                // Notice here that we don't know how many digits are in each value,
                // but we know that every value is separated by a comma.
                // So split the three values using comma as the separator.
                // The split function returns an object.
                rgb = color.split(',', 3); // Convert redValue to integer
                rgbColors[0] = parseInt(rgb[0]);
                // Convert greenValue to integer
                rgbColors[1] = parseInt(rgb[1]);
                // Convert blueValue to integer
                rgbColors[2] = parseInt(rgb[2]);
            }
            else if (color.substring(0, 1) == "#") {
                // This is simples because we know that every values is two 
                // hexadecimal digits.
                rgb = ["", "", ""];
                rgb[0] = color.substring(1, 3); // redValue
                rgb[1] = color.substring(3, 5); // greenValue
                rgb[2] = color.substring(5, 7); // blueValue
                // We need to convert the value into integers, 
                //   but the value is in hex (base 16)!
                // Fortunately, the parseInt function takes a second parameter 
                // signifying the base we're converting from.  
                rgbColors[0] = parseInt(rgb[0], 16);
                rgbColors[1] = parseInt(rgb[1], 16);
                rgbColors[2] = parseInt(rgb[2], 16);
            }
            return rgbColors;
        };
        return ColorConverter;
    })();
    Core.ColorConverter = ColorConverter;
})(Core || (Core = {}));
//# sourceMappingURL=core.js.map