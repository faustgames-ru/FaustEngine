using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace InputHook
{
    public struct MouseInput
    {
        public int XPosRelative;
        public int YPosRelative;
    }

    public class Mouse
    {
        public int XDelta;
        public int YDelta;
        public void Update()
        {
            XDelta = 0;
            YDelta = 0;
            while (_inputValues.Count > 0)
            {
                var input = _inputValues.Dequeue();
                XDelta += input.XPosRelative;
                YDelta += input.YPosRelative;
            }
        }

        public void Enqueue(MouseInput value)
        {
            _inputValues.Enqueue(value);
        }

        private Queue<MouseInput> _inputValues = new Queue<MouseInput>();
    }


    public class Input
    {
        public static Mouse Mouse = new Mouse();

        public static unsafe void Register(IntPtr windowHandle)
        {
            Rawinputdevice[] rid = new Rawinputdevice[1];
            rid[0].UsagePage = HIDUsagePage.Generic;
            rid[0].Usage = HIDUsage.Mouse;
            rid[0].Flags = RawInputDeviceFlags.None;
            rid[0].WindowHandle = windowHandle;
            WinApi.RegisterRawInputDevices(rid, 1, sizeof(Rawinputdevice));
        }

        public static void Update()
        {
            Mouse.Update();
        }


        public static unsafe void WinProc(ref Message m)
        {
            switch (m.Msg)
            {
                case (int) WindowsMessages.INPUT:
                {
                    var dwSize = sizeof(RawInput);
                    RawInput raw;
                    WinApi.GetRawInputData(m.LParam, RawInputCommand.Input, out raw, ref dwSize, sizeof (RawInputHeader));

                    if (raw.Header.Type == RawInputType.Mouse)
                    {
                        Mouse.Enqueue(new MouseInput
                        {
                            XPosRelative = raw.Data.Mouse.LastX,
                            YPosRelative = raw.Data.Mouse.LastY
                        });
                    }
                    break;
                }
            }
        }
    }
}
