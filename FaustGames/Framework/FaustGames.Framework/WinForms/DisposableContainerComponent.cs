using System;
using System.ComponentModel;

namespace FaustGames.Framework.WinForms
{
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