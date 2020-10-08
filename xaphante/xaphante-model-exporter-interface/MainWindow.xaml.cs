using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
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

        Dictionary<XaphanteFileType, Action> Progresses = new Dictionary<XaphanteFileType, Action>();

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

            foreach (string name in Enum.GetNames(typeof(XaphanteFileType)))
            {
                var g = new StackPanel {
                    Margin     = new Thickness(0, 0, 0, 0),
                    Background = null,
                };

                switch (Enum.Parse<XaphanteFileType>(name))
                {
                    case XaphanteFileType.ObjectFile: {
                        var o1 = new OutputFileControl { EXTENSION = FileWriter.XaphanteFileExtensionName(XaphanteFileType.ObjectFile) };
                        var i1 = new InputFileControl();
                        i1.OnOpen += path => {
                            o1.outputLBox.Text = Path.Combine(Path.GetDirectoryName(path), Path.GetFileNameWithoutExtension(path) + o1.EXTENSION);
                        };

                        g.Children.Add(i1);
                        g.Children.Add(o1);

                        this.Progresses.Add(XaphanteFileType.ObjectFile,
                            () => {
                                string inf = i1.inputLBox.Text;
                                string exf = o1.outputLBox.Text;
                                this.CancellationTokenMy = new CancellationTokenSource();
                                this.MainThread = new Thread(async () => {
                                    var exp = new ObjectExporter();
                                    exp.Init(StatusCallback, this.CancellationTokenMy.Token);

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
                            });
                    }

                        break;
                    case XaphanteFileType.ShaderProgram: {
                        var o1 = new OutputFileControl { EXTENSION = FileWriter.XaphanteFileExtensionName(XaphanteFileType.ShaderProgram) };
                        var i1 = new InputFileControl();
                        var i2 = new InputFileControl();

                        i1.OnOpen += path => {
                            o1.outputLBox.Text = Path.Combine(Path.GetDirectoryName(path)!, Path.GetFileNameWithoutExtension(path) + o1.EXTENSION);
                            string fag = Path.Combine(Path.GetDirectoryName(path)!, Path.GetFileNameWithoutExtension(path) + ".frag");
                            if (File.Exists(fag))
                            {
                                i2.inputLBox.Text = fag;
                            }
                        };

                        g.Children.Add(i1);
                        g.Children.Add(i2);
                        g.Children.Add(o1);

                        this.Progresses.Add(XaphanteFileType.ShaderProgram,
                            () => {
                                var vert = i1.inputLBox.Text;
                                var frag = i2.inputLBox.Text;
                                var outf = o1.outputLBox.Text;

                                this.CancellationTokenMy = new CancellationTokenSource();
                                this.MainThread = new Thread(async () => {
                                    var exp = new ShaderExporter();
                                    exp.Init(StatusCallback, this.CancellationTokenMy.Token);

                                    try
                                    {
                                        await exp.Run(vert, frag, new ShaderDescription[] { }, outf);
                                    }
                                    catch (Exception exception)
                                    {
                                        StatusCallback(new ProgressEvent(1, exception.Message));
                                        StatusCallback(new ProgressEvent(1, exception.StackTrace));
                                    }
                                });
                            });
                    }

                        break;
                    case XaphanteFileType.TextureFile: break;
                    default:                           throw new ArgumentOutOfRangeException();
                }

                var newTabItem = new TabItem {
                    Header          = name,
                    Name            = name,
                    Content         = g,
                    Background      = null,
                    Foreground      = this.progButton.Foreground,
                    BorderBrush     = this.progButton.BorderBrush,
                    BorderThickness = this.progButton.BorderThickness,
                };

                tabControl.Items.Add(newTabItem);
            }
        }

        private void Progress(object sender, RoutedEventArgs e)
        {
            if (this.MainThread != null && this.MainThread.IsAlive)
            {
                MessageBox.Show("Already Inprogress!");
                return;
            }

            if (this.tabControl.SelectedItem is TabItem ti)
            {
                this.Progresses[Enum.Parse<XaphanteFileType>(ti.Name)]?.Invoke();
            }

            this.MainThread?.Start();
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
    }
}
