module Core {

    export enum TableReplaceMode {
        ReplaceNone,
        ReplaceAll,
        ReplaceLarge,
    }

    export enum DisplayStyle {
        ByCss,
        BlackBackground,
        WhiteBackground,
        Inverse,
    }

    
    export class EpubLocation {
        public url: string;
        public idFrom: string;
        public idTo: string;

        public createClone(): EpubLocation {
            var clone = new EpubLocation();
            clone.url = this.url;
            clone.idFrom = this.idFrom;
            clone.idTo = this.idTo;
            return clone;
        }
    }

    export class Config {
        public static firstPageId: string = "_firstPageId_";
        public static lastPageId: string = "_lastPageId_";
        public static spanIdBase: string = "_span_";
        public static pageCenterLimit: number = 50;
        public static tablesWidthScale: number = 0.95;
        public static tablesMinWidth: number = 300;
        public static pagePadding: number = 10;
        public static pageHeightScale: number = 1.0;
        public static imagesMaxWScale: number = 0.95;
        public static imagesMaxHScale: number = 0.95;
        public static fontScale: number = 1.05;
        public static hyphynate: boolean = true;
        public static tableReplaceMode: TableReplaceMode = TableReplaceMode.ReplaceLarge;
        public static justifyParagraphs: boolean = false;
        public static clearTablesMargingsAndPaddings: boolean = true;
        public static displayStyle: DisplayStyle = DisplayStyle.BlackBackground;
        public static displayDebugHr: boolean = false;
    }

    export class EpubFiles {
        public static toc: string = "toc.ncx";
    }

    export enum LoadingStatus {
        Ok,    
        Failed
    }

    export class DomWalkerArgs {
        public node: Node;
        public breakWalk: boolean;
        public sckipChildren: boolean;
        public sckipNext: boolean;
        public isLeaf: boolean;
    }

    export interface IDomWalkerCallback {
        walkThrough(e: DomWalkerArgs): void;
    }

    export interface IDomWalkerFilter {
        (node: Node): boolean;
    }

    export class Bounds {
        left: number;
        right: number;
        top: number;
        bottom: number;
    }

    export interface IAbsolutBoundsDetector {
        getLocalAbsolutBounds(node: Node): Bounds;
        getAbsolutBounds(node: Node): Bounds;
    }


    export interface IDomWalker {
        filtNodes(doc: Document, filter: IDomWalkerFilter): Node[];
        walk(doc: Document, callback: IDomWalkerCallback): number
    }

    export interface ILoaderCallback {
        (status: LoadingStatus, fileText: string): void;
    }

    export interface IImageSizeCallback {
        (status: LoadingStatus, w: number, h: number): void;
    }

    export interface IImageDataCallback {
        (status: LoadingStatus, data: string): void;
    }

    export interface IImageLoadCallback {
        (): void;
    }

    export interface ILoader {
        load(fileName: string, resultCallback: ILoaderCallback): void;
    }

    export interface IHtmlParser {
        parse(text: string, mimeType: string) : Document;
    }

    export interface IDomModifier {
        updateFontSize(doc: Document): void;
        setDisplayStyle(doc: Document): void;
        clearCss(doc: Document): void;
        updateImageSources(doc: Document, baseUrl: string, callback: IImageLoadCallback): void;
        updateCssRefs(doc: Document, baseUrl: string): void;
        createSpansFormWords(doc: Document): void;
        setTableWidth(doc: Document): void;
        setTdWidth(doc: Document): void;
        setImageSize(doc: Document): void;
        addCss(doc: Document, href: string): void;
        justifyParagraphs(doc: Document): void;
    }

    export interface IDomSynchronizer {
        synchronize(target: Document, source: Document): void;
    }

    export class Path {
        public static getDirectory(path: string) {
            var items = path.split("/");
            var result: string = "";
            for (var i = 0; i < (items.length - 1); i++) {
                result += items[i] + "/";
            }
            return result;
        }
    }


    export class Services {
        public static loader: ILoader;
        public static domParser: IHtmlParser;
        public static domSynchronizer: IDomSynchronizer;
        public static domModifier: IDomModifier;
        public static domWalker: IDomWalker;
        public static absolutBoundsDetector: IAbsolutBoundsDetector;
    }

    export class ColorConverter {
        public static convertColor(color: string): number[] {
            var rgbColors: number[] = [0, 0, 0];

            ///////////////////////////////////
            // Handle rgb(redValue, greenValue, blueValue) format
            //////////////////////////////////
            var rgb: string[];
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
            ////////////////////////////////
            // Handle the #RRGGBB' format
            ////////////////////////////////
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
        }
    }
}