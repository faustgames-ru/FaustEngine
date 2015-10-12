/// <reference path="core.ts" />

module PlatformWeb {
    import ILoader = Core.ILoader;
    import IImageSizeCallback = Core.IImageSizeCallback;
    import IImageDataCallback = Core.IImageDataCallback;
    import ILoaderCallback = Core.ILoaderCallback;
    import LoadingStatus = Core.LoadingStatus;

    export class Loader implements ILoader {
        public load(fileName: string, resultCallback: ILoaderCallback): void {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", fileName, true);
            xhr.onreadystatechange = () => {
                if (xhr.readyState == 4) {
                    if ((xhr.status == 200)) {
                        resultCallback(LoadingStatus.Ok, xhr.responseText);
                    } else {
                        resultCallback(LoadingStatus.Failed, xhr.responseText);
                    }
                }
            }
            xhr.send(null);
        }
    }
}