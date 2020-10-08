using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using Assimp;
using Assimp.Configs;

namespace xaphante_model_exporter_lib
{
    public class ObjectExporter : IXaphanteExporter
    {
        public SceneDataExportable dataHolder = new SceneDataExportable();

        /// <inheritdoc />
        public void Init(Action<ProgressEvent> statusCallback, CancellationToken token) => ((IXaphanteExporter) this).InitTemplate(statusCallback, token);

        /// <inheritdoc />
        public void UpdateLast(string msg) => ((IXaphanteExporter) this).UpdateLastTemplate(msg);

        /// <inheritdoc />
        public void UpdateNew(string msg, int now, int max) => ((IXaphanteExporter) this).UpdateNewTemplate(msg, now, max);

        /// <inheritdoc />
        public ProgressEvent Last { get; set; }

        /// <inheritdoc />
        public Action<ProgressEvent> StatusCallback { get; set; }

        /// <inheritdoc />
        public CancellationToken Token { get; set; }

        public FileWriter Writer { get; set; }

        public async Task<bool> Run(string importFile, string exportFile)
        {
            bool success = ImportScene(importFile);
            if (!success)
            {
                UpdateNew("Import Failed! Exiting...", 1, 1);
                Thread.Sleep(300);
                return false;
            }

            this.Token.ThrowIfCancellationRequested();

            UpdateNew("Waiting For Export...", 1, 1);

            this.Writer = new FileWriter(exportFile, XaphanteFileType.ObjectFile);
            Thread.Sleep(300);

            this.Token.ThrowIfCancellationRequested();

            success = await ExportToFile(exportFile);

            if (!success)
                UpdateNew("Export Failed! Exiting...", 1, 1);

            return success;
        }

        #region Export

        private readonly Config<uint> config = new Config<uint>();

        private async Task<bool> ExportToFile(string exportFile)
        {
            UpdateNew("Exporting To: " + exportFile, 0, 1);

            this.Writer.Open();

            var cancellationToken = this.Token;
            this.Writer.WriteHeader(ref cancellationToken);
            this.Writer.Writer.Write(this.config.Size);

            UpdateNew("Writing Vertices...", 1, 100);
            this.Writer.WriteList(this.dataHolder.Positions, ref cancellationToken);

            cancellationToken.ThrowIfCancellationRequested();

            UpdateNew("Writing Indices...", 1, 100);
            this.Writer.WriteList(this.dataHolder.Indices, ref cancellationToken);

            this.Writer.Close();

            UpdateNew("Finished !", 1, 1);

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
                        scene = importer.ImportFile(importFile, PostProcessSteps.Triangulate | PostProcessSteps.GenerateSmoothNormals | PostProcessSteps.OptimizeMeshes | PostProcessSteps.OptimizeGraph | PostProcessSteps.JoinIdenticalVertices | PostProcessSteps.ImproveCacheLocality);
                    }
                    catch (Exception e)
                    {
                        UpdateLast(e.Message);
                    }
                },
                this.Token);

            UpdateNew("Importing...", 0, 1);

            int       i   = 0;
            const int max = 100;
            var       r   = new Random();

            while (!task.IsCompleted)
            {
                UpdateNew("", i++ % max, max);
                Thread.Sleep(1 + (int) (r.NextDouble() * i % max / 10));
            }

            if (scene == null || (scene.SceneFlags & SceneFlags.Incomplete) != 0 || scene.RootNode == null)
            {
                UpdateLast("Scene is Not Valid");
                return false;
            }

            ProcessNode(scene.RootNode, scene, 0);
            UpdateNew("Finished Import!", 1, 1);
            return true;
        }

        private void ProcessNode(Node node, Scene scene, int layer)
        {
            this.Token.ThrowIfCancellationRequested();

            UpdateLast("Depth: " + layer);
            ProcessData(node, scene);

            foreach (var children in node.Children)
            {
                this.Token.ThrowIfCancellationRequested();

                ProcessNode(children, scene, layer + 1);
            }
        }

        private void ProcessData(Node node, Scene scene)
        {
            foreach (int meshIndex in node.MeshIndices)
            {
                ProcessMesh(scene.Meshes[meshIndex], scene);
                UpdateNew("Processing...", meshIndex, scene.MeshCount);
            }
        }

        private void ProcessMesh(Mesh sceneMesh, Scene scene)
        {
            this.Token.ThrowIfCancellationRequested();
            this.dataHolder.Positions.AddRange(GenVertices(sceneMesh));
            this.dataHolder.Indices.AddRange(sceneMesh.GetUnsignedIndices());

            //if (sceneMesh.HasFaces)
            //{
            //    foreach (var face in sceneMesh.Faces.Where(face => face.IndexCount > 0 && face.Indices != null))
            //    {
            //        foreach (uint index in face.Indices)
            //            this.dataHolder.Indices.Add(index);
            //    }
            //}

            UpdateLast("Processed");
        }

        private IEnumerable<Position> GenVertices(Mesh sceneMesh)
        {
            if (sceneMesh.HasNormals)
                for (var i = 0; i < sceneMesh.VertexCount; i++)
                {
                    yield return new Position(sceneMesh.Vertices[i], sceneMesh.Normals[i]);
                }
            else
                for (var i = 0; i < sceneMesh.VertexCount; i++)
                {
                    yield return new Position(sceneMesh.Vertices[i], default);
                }
        }

        #endregion
    }
}
