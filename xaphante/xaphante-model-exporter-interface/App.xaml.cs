using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace xaphante_model_exporter_interface
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static void ExplorerShow(string p)
        {
            string args = $"/e, /select, \"{p}\"";

            ProcessStartInfo info = new ProcessStartInfo();
            info.FileName  = "explorer";
            info.Arguments = args;
            Process.Start(info);
        }
        
    }
}
