using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using llge;
using OpenGLWindow;

namespace GameSampleWindowForms
{
    public partial class Main : Form
    {
        private readonly OGLWindow _oglWindow;
        private readonly RenderSystem _renderSystem;
        public Main()
        {
            InitializeComponent();

            var llgeFactory = llge.llge.CreateFactory();
            _renderSystem = llgeFactory.CreateRenderSystem();

            _oglWindow = new OGLWindow(this, Width, Height);
            if (components == null)
                components = new Container();
            components.Add(new DisposableContainerComponent(_oglWindow));
            components.Add(new DisposeActionContainerComponent(() => _renderSystem.Dispose()));
            Application.Idle += ApplicationOnIdle;
        }

        private Stopwatch _stopwatch;

        private void DoUpdate()
        {
            var delta = 0.01f;
            if (_stopwatch != null)
            {
                _stopwatch.Stop();
                delta = (float)_stopwatch.Elapsed.TotalSeconds;
            }
            _stopwatch = Stopwatch.StartNew();
            _oglWindow.Render(delta);
            _renderSystem.Render();
            _oglWindow.SwapOpenGLBuffers();
        }

        private void ApplicationOnIdle(object sender, EventArgs eventArgs)
        {
            DoUpdate();
        }

        private void _timer_Tick(object sender, EventArgs e)
        {
            DoUpdate();
        }
    }

    internal class DisposeActionContainerComponent : IComponent
    {
        private readonly Action _disposeAction;
        public DisposeActionContainerComponent(Action disposeAction)
        {
            _disposeAction = disposeAction;
        }

        public void Dispose()
        {
            _disposeAction();
        }

        public ISite Site { get; set; }
        public event EventHandler Disposed;
    }

    internal class DisposableContainerComponent : IComponent
    {
        private readonly IDisposable _disposable;
        public bool IsDisposed;
        public DisposableContainerComponent(IDisposable disposable)
        {
            _disposable = disposable;
        }


        public void Dispose()
        {
            IsDisposed = true;
            _disposable.Dispose();
        }

        public ISite Site { get; set; }
        public event EventHandler Disposed;
    }
}
