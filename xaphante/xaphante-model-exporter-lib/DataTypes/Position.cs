using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Numerics;
using System.Reflection;
using System.Runtime.InteropServices;
using Assimp;
using Matrix4x4 = Assimp.Matrix4x4;
using ThreadState = System.Diagnostics.ThreadState;

namespace xaphante_model_exporter_lib
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Position
    {
        public Vector3D pos;

        public Vector3D normal;

        public Position(Vector3D pos, Vector3D normal)
        {
            this.normal = normal;
            this.pos    = pos;
        }
    }
}
