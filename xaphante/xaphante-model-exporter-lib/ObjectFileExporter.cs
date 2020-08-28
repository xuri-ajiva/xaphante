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
        private          CancellationToken     token;

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

        private FileWriter writer;

        public async Task<bool> Run(string importFile, string exportFile)
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

            this.writer = new FileWriter(exportFile, XaphanteFileType.ObjectFile);
            Thread.Sleep(300);

            this.token.ThrowIfCancellationRequested();

            success = await ExportToFile(exportFile);

            if (!success)
                UpdateAdd(1, 1, "Export Failed! Exiting...");

            return success;
        }

        #region Export

        Config<uint> config = new Config<uint>();

        private async Task<bool> ExportToFile(string exportFile)
        {
            UpdateAdd(0, 1, "Exporting To: " + exportFile);

            this.writer.Open();

            this.writer.WriteHeader(ref this.token);
            this.writer.Writer.Write(this.config.Size);

            UpdateAdd(1, 100, "Writing Vertices...");
            this.writer.WriteList(this.dataHolder.Positions, ref this.token);

            this.token.ThrowIfCancellationRequested();

            UpdateAdd(1, 100, "Writing Indices...");
            this.writer.WriteList(this.dataHolder.Indices, ref this.token);

            this.writer.Close();

            UpdateAdd(1, 1, "Finished !");

            return true;
        }

        #endregion

        #region Import

        private bool ImportScene(string importFile)
        {
            if (!File.Exists(importFile))
                throw new ArgumentException("File Dose Not Exists");

            var importer = new AssimpContext();
            importer.SetConfig(new NormalSmoothingAngleConfig(66.0f));
            Scene scene = default;
            var task = Task.Run(() => {
                    try
                    {
                        scene = importer.ImportFile(importFile, PostProcessSteps.Triangulate | PostProcessSteps.OptimizeMeshes | PostProcessSteps.OptimizeGraph | PostProcessSteps.JoinIdenticalVertices | PostProcessSteps.ImproveCacheLocality);
                    }
                    catch (Exception e)
                    {
                        Update(e.Message);
                    }
                },
                this.token);

            UpdateAdd(0, 1, "Importing...");
            
            int       i   = 0;
            const int max = 100;
            var       r   = new Random();
            
            while (!task.IsCompleted)
            {
                UpdateAdd(i++ % max, max, "");
                Thread.Sleep(1 + (int) (r.NextDouble() * i%max / 10));
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
            this.dataHolder.Indices.AddRange(sceneMesh.GetUnsignedIndices());

            //if (sceneMesh.HasFaces)
            //{
            //    foreach (var face in sceneMesh.Faces.Where(face => face.IndexCount > 0 && face.Indices != null))
            //    {
            //        foreach (uint index in face.Indices)
            //            this.dataHolder.Indices.Add(index);
            //    }
            //}

            Update("Processed");
        }

        #endregion
    }
}
