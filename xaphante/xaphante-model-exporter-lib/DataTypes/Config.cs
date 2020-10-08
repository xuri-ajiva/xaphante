using System.Runtime.InteropServices;

namespace xaphante_model_exporter_lib
{
    public class Config <T>
    {
        public T   TX   { get; set; }
        public int Size => Marshal.SizeOf<T>();
    }
}