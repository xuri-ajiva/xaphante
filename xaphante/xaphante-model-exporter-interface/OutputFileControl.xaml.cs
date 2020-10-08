using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
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

namespace xaphante_model_exporter_interface
{
    /// <summary>
    /// Interaction logic for OutputDileControl.xaml
    /// </summary>
    public partial class OutputFileControl : UserControl
    {
        public event Action<string> OnOpen;

        public OutputFileControl()
        {
            InitializeComponent();
        }

        private void OpenOutput(object senOpenOutputEventArgs, RoutedEventArgs e)
        {
            var sf = new SaveFileDialog { DefaultExt = this.EXTENSION, AddExtension = true };
            if (sf.ShowDialog() != true)
                return;

            this.outputLBox.Text = sf.FileName;
        }

        private void ShowOutput(object sender, RoutedEventArgs e)
        {
            App.ExplorerShow(this.outputLBox.Text);
        }

        public string EXTENSION { get; set; }
    }
}
