using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Search;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236
using Windows.Web;
using Ionic.Zip;

namespace EPubViewer.Controls
{
    public enum BookmarkType
    {
        None,
        Page,
        Selection
    }

    public enum EpubLoadingStatus
    {
        None,
        Ok,
        Error
    }

    public class EpubLocation
    {
        public string Url;
        public string IdFrom;
        public string IdTo;
    }

    public interface IEpubView
    {
        Task<EpubLoadingStatus> LoadFromStream(Stream epubStream);
        Task Open(EpubLocation location);
        Task<bool> NextPageAvaliable();
        Task<bool> PrevPageAvaliable();
        Task<bool> OpenNextPage();
        Task<bool> OpenPrevPage();
        Task<EpubLocation> GetBookmark(BookmarkType type);
    }

    public class StreamResolver : IUriToStreamResolver, IDisposable
    {
        private ZipFile _file;
        private string _fileName;

        public StreamResolver(string fileName)
        {
            _fileName = fileName;
        }

        public IAsyncOperation<IInputStream> UriToStreamAsync(Uri uri)
        {
            if (uri == null)
            {
                throw new Exception();
            }
            string path = uri.AbsolutePath;
            return GetContent(path).AsAsyncOperation();
        }

        private async Task<IInputStream> GetContent(string path)
        {
            var memoryStream = new MemoryStream();
            var writer = new StreamWriter(memoryStream);
            await writer.WriteAsync("<!DOCTYPE html> <html lang='en'> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <meta charset='utf-8' /><title>TypeScript HTML App</title></head><body><p>Text!!!!</p></body></html>");
            await writer.FlushAsync();
            memoryStream.Position = 0;
            var inputStream = memoryStream.AsInputStream();
            return inputStream;

            /*
            try
            {
                if (_file == null)
                {
                    var localUri = new Uri(_fileName);
                    var file = await StorageFile.GetFileFromApplicationUriAsync(localUri);
                    var stream = (await file.OpenReadAsync()).AsStreamForRead();
                    _file = ZipFile.Read(stream);
                }
                var entries = _file.Entries;
                foreach (ZipEntry e in entries)
                {
                    if (("/" + e.FileName) == path)
                    {
                        using (var entryStream = e.OpenReader())
                        {
                            var memoryStream = new MemoryStream();
                            var writer = new StreamWriter(memoryStream, Encoding.UTF8);
                            await writer.WriteAsync("<p>text</p>");
                            await writer.FlushAsync();
                            memoryStream.Position = 0;
                            return memoryStream.AsInputStream();
                        }
                        
                        //var resultStream = entryStream.AsInputStream();
                        //return resultStream;
                    }
                }
            }
            catch (Exception e)
            {
            }
            return null;
             */ 
        }

        public void Dispose()
        {
            _file.Dispose();
        }
    }

    public sealed partial class EpubView : UserControl
    {
        public EpubView()
        {
            InitializeComponent();
        }

        public void Load()
        {
            var uri = _webView.BuildLocalStreamUri("MyTag", "/OEBPS/Text/1.html");

            _webView.NavigationCompleted += WebViewOnNavigationCompleted;
            _webView.NavigationFailed += WebViewOnNavigationFailed;
            _webView.NavigationStarting += WebViewOnNavigationStarting;
            //_webView.NavigateToLocalStreamUri(uri, new StreamResolver("ms-appx:///Assets/QueryLanguage.epub"));
            _webView.NavigateToString("<!DOCTYPE html> <html lang='en'> <head> <meta name='viewport' content='width=device-width, initial-scale=1'> <meta charset='utf-8' /><title>TypeScript HTML App</title></head><body><p>Text!!!!</p></body></html>");
        }

        private void WebViewOnNavigationStarting(WebView sender, WebViewNavigationStartingEventArgs args)
        {
        }

        private void WebViewOnNavigationFailed(object sender, WebViewNavigationFailedEventArgs webViewNavigationFailedEventArgs)
        {
        }

        private void WebViewOnNavigationCompleted(WebView sender, WebViewNavigationCompletedEventArgs args)
        {
        }
    }
}
