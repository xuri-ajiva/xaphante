using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using Assimp;
using Assimp.Configs;
using Matrix4x4 = Assimp.Matrix4x4;
using ThreadState = System.Diagnostics.ThreadState;

namespace xaphante_model_exporter_lib
{
    public class SceneDataExportable
    {
        public List<Vector3D> Positions = new List<Vector3D>();
        public List<UInt32>   Indices   = new List<UInt32>();
    }

    public class Config <T>
    {
        public T   TX   { get; set; }
        public int Size => Marshal.SizeOf<T>();
    }

    public class ProgressEvent
    {
        public double Percentage { get; }

        public string Status { get; }

        public ProgressEvent(double percentage, string status)
        {
            this.Percentage = percentage;
            this.Status     = status;
        }
    }

    public class Exporter
    {
        private readonly Action<ProgressEvent> statusCallback;
        private readonly CancellationToken     token;

        private ProgressEvent last = new ProgressEvent(0, "Initializing");

        public Exporter(Action<ProgressEvent> statusCallback, CancellationToken token)
        {
            this.statusCallback = statusCallback;
            this.token          = token;
        }

        void Update(string msg)
        {
            this.statusCallback?.Invoke(new ProgressEvent(this.last.Percentage, msg));
        }

        void UpdateAdd(int now, int max, string msg)
        {
            this.statusCallback?.Invoke(new ProgressEvent(now / (double) max, msg));
        }

        public SceneDataExportable dataHolder = new SceneDataExportable();

        public async Task<bool> Export(string importFile, string exportFile)
        {
            bool success = ImportScene(importFile);
            if (!success)
            {
                UpdateAdd(1, 1, "Import Failed! Exiting...");
                Thread.Sleep(300);
                return false;
            }

            this.token.ThrowIfCancellationRequested();

            UpdateAdd(1, 1, "Waiting For Export...");

            Thread.Sleep(300);

            this.token.ThrowIfCancellationRequested();

            success = await ExportToFile(exportFile);

            if (!success)
                UpdateAdd(1, 1, "Export Failed! Exiting...");

            return success;
        }

        #region Export

        Config<uint> config = new Config<uint>();

        byte[] ListToBytes <T>(List<T> lst)
        {
            int LS = lst.Count;
            int TS = Marshal.SizeOf<T>();

            byte[] arr = new byte[TS * LS];

            IntPtr ptr = Marshal.AllocHGlobal(arr.Length);

            UpdateAdd(0, LS, "Exporting List...");
            for (int i = 0; i < LS; i++)
            {
                UpdateAdd(i, LS, lst[i].ToString());
                this.token.ThrowIfCancellationRequested();

                Marshal.StructureToPtr(lst[i], ptr + i * TS, true);
            }

            Marshal.Copy(ptr, arr, 0, arr.Length);
            Marshal.FreeHGlobal(ptr);
            return arr;
        }

        private async Task<bool> ExportToFile(string exportFile)
        {
            UpdateAdd(0, 1, "Exporting To: " + exportFile);
            if (File.Exists(exportFile))
            {
                Update("Output File exists Moving to .rnd.old!");
                Thread.Sleep(1000);
                File.Move(exportFile, exportFile + new Random().NextDouble() + ".old");
            }

            var st = File.Open(exportFile, FileMode.CreateNew);

            await using var bf = new BinaryWriter(st);

            WriteHeader(bf, exportFile);

            UpdateAdd(1, 100, "Writing Vertices...");
            bf.Write(this.dataHolder.Positions.Count);
            bf.Write(ListToBytes(this.dataHolder.Positions));

            this.token.ThrowIfCancellationRequested();

            UpdateAdd(1, 100, "Writing Indices...");
            bf.Write(this.dataHolder.Indices.Count);
            bf.Write(ListToBytes(this.dataHolder.Indices));

            await st.FlushAsync(this.token);
            st.Close();

            UpdateAdd(1, 1, "Finished !");

            return true;
        }

        private void WriteHeader(BinaryWriter bf, string exportFile)
        {
            this.token.ThrowIfCancellationRequested();

            UpdateAdd(1, 100, "Writing Header...");

            bf.Write("BSF");
            bf.Write(Path.GetFileNameWithoutExtension(exportFile));

            bf.Write(this.config.Size);
        }

        #endregion

        #region Import

        private bool ImportScene(string importFile)
        {
            if (!File.Exists(importFile))
                throw new ArgumentException("File Dose Not Exists");

            var importer = new AssimpContext();
            importer.SetConfig(new NormalSmoothingAngleConfig(66.0f));
            Scene scene;
            try
            {
                scene = importer.ImportFile(importFile, PostProcessSteps.Triangulate | PostProcessSteps.OptimizeMeshes | PostProcessSteps.OptimizeGraph | PostProcessSteps.JoinIdenticalVertices | PostProcessSteps.ImproveCacheLocality);
            }
            catch (Exception e)
            {
                Update(e.Message);
                return false;
            }

            if (scene == null || (scene.SceneFlags & SceneFlags.Incomplete) != 0 || scene.RootNode == null)
            {
                Update("Scene is Not Valid");
                return false;
            }

            ProcessNode(scene.RootNode, scene, 0);
            UpdateAdd(1, 1, "Finished Import!");
            return true;
        }

        private void ProcessNode(Node node, Scene scene, int layer)
        {
            this.token.ThrowIfCancellationRequested();

            Update("Depth: " + layer);
            ProcessData(node, scene);

            foreach (var children in node.Children)
            {
                this.token.ThrowIfCancellationRequested();

                ProcessNode(children, scene, layer + 1);
            }
        }

        private void ProcessData(Node node, Scene scene)
        {
            foreach (int meshIndex in node.MeshIndices)
            {
                ProcessMesh(scene.Meshes[meshIndex], scene);
                UpdateAdd(meshIndex, scene.MeshCount, "Processing...");
            }
        }

        private void ProcessMesh(Mesh sceneMesh, Scene scene)
        {
            this.token.ThrowIfCancellationRequested();
            this.dataHolder.Positions.AddRange(sceneMesh.Vertices);
            //this.dataHolder.Indices.AddRange(sceneMesh.GetUnsignedIndices());

            if (sceneMesh.HasFaces)
            {
                foreach (var face in sceneMesh.Faces.Where(face => face.IndexCount > 0 && face.Indices != null))
                {
                    foreach (uint index in face.Indices)
                        this.dataHolder.Indices.Add(index);
                }
            }

            Update("Processed");
        }

        #endregion
    }
}
