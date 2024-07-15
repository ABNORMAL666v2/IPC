using System.Runtime.InteropServices;
namespace IPC_Installer
{
    internal static class Program
    {
        [DllImport("kernel32.dll")]
        internal static extern Boolean AllocConsole();
        [STAThread]
        private static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                AllocConsole();
                ComWrappers.RegisterForMarshalling(WinFormsComInterop.WinFormsComWrappers.Instance);

                ApplicationConfiguration.Initialize();
                Application.Run(new InstallerForm());
            }
            else
            {
                AllocConsole();
            }

        }
    }
}