using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using xaphante_model_exporter_lib;
using Path = System.IO.Path;

namespace xaphante_model_exporter_interface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private const string EXTENSION = ".bsf";

        public MainWindow()
        {
            InitializeComponent();
            FileWriter.Infos += delegate(string s) {
                this.Dispatcher?.Invoke(() => {
                    if (string.IsNullOrEmpty(s))
                    {
                        return;
                    }

                    this.status.AppendText(s + "\n");
                    this.status.Focus();
                    this.status.CaretIndex = this.status.Text.Length;
                    this.status.ScrollToEnd();
                });
            };
            FileWriter.UpdateStatus += delegate(double d) {
                this.Dispatcher?.Invoke(() => {
                    this.progBar2.Value = d * this.progBar2.Maximum;
                });
            };
        }

        private void Progress(object sender, RoutedEventArgs e)
        {
            var inf = this.inputLBox.Text;
            var exf = this.outputLBox.Text;

            if (this.MainThread != null && this.MainThread.IsAlive)
            {
                MessageBox.Show("Already Inprogress!");
                return;
            }

            CancellationTokenMy = new CancellationTokenSource();
            this.MainThread = new Thread(async () => {
                var exp = new Exporter(StatusCallback, this.CancellationTokenMy.Token);

                try
                {
                    await exp.Run(inf, exf);
                }
                catch (Exception exception)
                {
                    StatusCallback(new ProgressEvent(1, exception.Message));
                    StatusCallback(new ProgressEvent(1, exception.StackTrace));
                }
            });

            this.MainThread.Start();
        }

        public CancellationTokenSource CancellationTokenMy { get; set; }

        public Thread MainThread { get; set; }

        private void StatusCallback(ProgressEvent obj)
        {
            this.Dispatcher?.Invoke(() => {
                this.progBar.Value = obj.Percentage * this.progBar.Maximum;

                if (string.IsNullOrEmpty(obj.Status))
                {
                    return;
                }

                this.status.AppendText(obj.Status + "\n");
                this.status.Focus();
                this.status.CaretIndex = this.status.Text.Length;
                this.status.ScrollToEnd();
            });
        }

        private void OpenInput(object sender, RoutedEventArgs e)
        {
            var of = new OpenFileDialog();
            if (of.ShowDialog() != true)
                return;

            this.inputLBox.Text  = of.FileName;
            this.outputLBox.Text = Path.Combine(Path.GetDirectoryName(of.FileName), Path.GetFileNameWithoutExtension(of.FileName) + EXTENSION);
        }

        private void OpenOutput(object senOpenOutputEventArgs, RoutedEventArgs e)
        {
            var sf = new SaveFileDialog { DefaultExt = EXTENSION, AddExtension = true };
            if (sf.ShowDialog() != true)
                return;

            this.outputLBox.Text = sf.FileName;
        }

        private void Abort(object sender, RoutedEventArgs e)
        {
            if (this.MainThread == null)
            {
                MessageBox.Show("No Task In Progress");
            }
            else
            {
                this.CancellationTokenMy?.Cancel();
            }
        }

        private void ShowInput(object sender, RoutedEventArgs e)
        {
            ExplorerShow(this.inputLBox.Text);
        }

        private void ShowOutput(object sender, RoutedEventArgs e)
        {
            ExplorerShow(this.outputLBox.Text);
        }

        void ExplorerShow(string p)
        {
            string args = string.Format("/e, /select, \"{0}\"", p);

            ProcessStartInfo info = new ProcessStartInfo();
            info.FileName  = "explorer";
            info.Arguments = args;
            Process.Start(info);
        }
    }
}
