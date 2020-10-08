using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using ThreadState = System.Diagnostics.ThreadState;

namespace xaphante_model_exporter_lib
{
    public class FileWriter
    {
        public static string XaphanteFileTypeName(XaphanteFileType type) =>
            type switch {
                XaphanteFileType.ObjectFile    => "BOBF",
                XaphanteFileType.ShaderProgram => "BSPF",
                XaphanteFileType.TextureFile   => "BTXF",
                _                              => "BUKF"
            };
        public static string XaphanteFileExtensionName(XaphanteFileType type) =>
            type switch {
                XaphanteFileType.ObjectFile    => ".bobf",
                XaphanteFileType.ShaderProgram => ".bspf",
                XaphanteFileType.TextureFile   => ".btxf",
                _                              => ".bukf"
            }; 
        
        public static event Action<double> UpdateStatus;
        public static event Action<string> Infos;

        protected virtual void OnUpdateStatus(double obj) => UpdateStatus?.Invoke(obj);

        protected virtual void OnInfos(string obj) => Infos?.Invoke(obj);

        protected virtual void OnUpdateStatus(int value, int max) => UpdateStatus?.Invoke(value / (double) max);

        private readonly string path;

        private readonly XaphanteFileType type;

        public FileWriter(string path, XaphanteFileType type)
        {
            this.path = path;
            this.type = type;
        }

        public BinaryWriter Writer { get; private set; }

        private void ThrowIfWriterIsNull()
        {
            if (this.Writer == null) throw new ArgumentException("Open the Path First!");
        }

        public void Open()
        {
            if (File.Exists(this.path))
            {
                OnInfos("Output File exists Moving to .rnd.old!");
                Thread.Sleep(1000);
                File.Move(this.path, this.path + new Random().NextDouble() + ".old");
            }

            this.Writer = new BinaryWriter(File.Open(this.path, FileMode.CreateNew));
        }

        public void Close()
        {
            ThrowIfWriterIsNull();

            this.Writer.Flush();
            this.Writer.Close();
            this.Writer = default;
        }

        public void WriteHeader(ref CancellationToken token)
        {
            token.ThrowIfCancellationRequested();
            ThrowIfWriterIsNull();

            OnInfos("Writing Header...");

            this.Writer.Write(XaphanteFileTypeName(this.type));
            this.Writer.Write(Path.GetFileNameWithoutExtension(this.path)!);
        }

        public void WriteList <T>(List<T> lst, ref CancellationToken token)
        {
            ThrowIfWriterIsNull();

            int ls = lst.Count;
            int ts = Marshal.SizeOf<T>();

            int onPercent = (int) (ls / 100d);
            onPercent = onPercent == 0 ? 1 : onPercent;

            byte[] arr = new byte[ts * ls];

            IntPtr ptr = Marshal.AllocHGlobal(arr.Length);

            for (int i = 0; i < ls; i++)
            {
                Marshal.StructureToPtr(lst[i], ptr + i * ts, true);

                if (i % onPercent != 0)
                    continue;

                OnUpdateStatus(i, ls);
                token.ThrowIfCancellationRequested();
            }
            OnUpdateStatus(1, 2);
            
            Marshal.Copy(ptr, arr, 0, arr.Length);
            Marshal.FreeHGlobal(ptr);
            OnUpdateStatus(1, 1);

            this.Writer.Write(ls);
            this.Writer.Write(arr);
        }


    }
}
