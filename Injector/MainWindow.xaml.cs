using Microsoft.Win32;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Input;

namespace KarambitInnit
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("kernel32.dll")]
        static extern IntPtr GetConsoleWindow();

        [DllImport("user32.dll")]
        static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        private const int SW_HIDE = 0x0;

        public string dllName = "";

        public MainWindow()
        {
            IntPtr handle = GetConsoleWindow();
            ShowWindow(handle, SW_HIDE);

            InitializeComponent();

            Verbose.VerticalScrollBarVisibility = System.Windows.Controls.ScrollBarVisibility.Auto;
            Verbose.Text = "\n    [Injection Verbose Will Appear Here:]\n\n";

            string result = Utils.setupFilesystem();

            if (result == "created")
            {
                Verbose.AppendText(Utils.time() + "Successfully created config files and directories.");
            }
            else if (result == "failed")
            {
                Verbose.AppendText(Utils.time() + "Failed to create config files and directories. Are you in a protected directory?");
            }
            else if (result == "skipped")
            {
                Verbose.AppendText(Utils.time() + "Sucessfully read config files.");
            }

            Verbose.AppendText("\n" + Utils.time() + "Gave magic powers to the potato.");
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (dllName != "")
            {
                bool success = Injector.Start(dllName);
                //bool success = Injector.Start("DLLs\\" + dllName.Split('\\')[dllName.Split('\\').Length - 1]);
                //bool success = Injector.Start(@"DLLs\knifechanger2.dll");
                //bool success = Injector.Start(Directory.GetCurrentDirectory() + @"\DLLs\saul.dll");

                if (success)
                {
                    Verbose.AppendText("\n" + Utils.time() + "Successfully injected DLL into CS:GO!");
                }
                else
                {
                    Verbose.AppendText("\n" + Utils.time() + "Failed to inject into CS:GO. Read below for more information:");
                }
            }
            else
            {
                Verbose.AppendText("\n" + Utils.time() + "No DLL file selected!");
            }
        }

        private void Image_MouseDown(object sender, MouseButtonEventArgs e)
        {
            MessageBox.Show("Potato?");
        }

        private void Verbose_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        {
            Verbose.Focus();
            Verbose.CaretIndex = Verbose.Text.Length;
            Verbose.ScrollToEnd();
        }

        private void dllSelectButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog fileSelector = new OpenFileDialog();
            fileSelector.DefaultExt = ".dll";
            fileSelector.Filter = "Dynamic Link Library (*.dll)|*.dll";
            fileSelector.InitialDirectory = Directory.GetCurrentDirectory() + @"\DLLs";
            fileSelector.RestoreDirectory = true;
            
            Nullable<bool> file = fileSelector.ShowDialog();

            if (file == true)
            {
                dllName = fileSelector.FileName;
                Verbose.AppendText("\n" + Utils.time() + "Chose DLL: '" + dllName + "'");
            }
        }
    }
}
