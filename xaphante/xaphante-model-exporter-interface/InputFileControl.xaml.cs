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
using Path = System.IO.Path;

namespace xaphante_model_exporter_interface
{
    /// <summary>
    /// Interaction logic for InputFileControl.xaml
    /// </summary>
    public partial class InputFileControl
    {
        public event Action<string> OnOpen;

        public InputFileControl()
        {
            InitializeComponent();
        }

        private void OpenInput(object sender, RoutedEventArgs e)
        {
            var of = new OpenFileDialog();
            if (of.ShowDialog() != true)
                return;

            this.inputLBox.Text = of.FileName;
            this.OnOpen?.Invoke(of.FileName);
        }

        private void ShowInput(object sender, RoutedEventArgs e)
        {
            App.ExplorerShow(this.inputLBox.Text);
        }
    }
}
