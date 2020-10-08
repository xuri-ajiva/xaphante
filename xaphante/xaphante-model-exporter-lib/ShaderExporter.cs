using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using Assimp;
using Assimp.Configs;

namespace xaphante_model_exporter_lib
{
    public class ShaderExporter : IXaphanteExporter
    {
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

        public async Task<bool> Run(string vertexShader, string fragmentShader, IEnumerable<ShaderDescription> shaderUniforms, string outputFile)
        {
            bool success = ImportShaders(vertexShader, fragmentShader);
            if (!success)
            {
                UpdateNew("Import Failed! Exiting...", 1, 1);
                Thread.Sleep(300);
                return false;
            }

            this.Token.ThrowIfCancellationRequested();

            UpdateNew("Waiting For Export...", 1, 1);

            this.Writer = new FileWriter(outputFile, XaphanteFileType.ShaderProgram);
            Thread.Sleep(300);

            this.Token.ThrowIfCancellationRequested();

            success = await ExportToFile(outputFile);

            if (!success)
                UpdateNew("Export Failed! Exiting...", 1, 1);

            return success;
        }

        #region Export

        private readonly Config<uint> config = new Config<uint>();

        private async Task<bool> ExportToFile(string exportFile)
        {
            var vertList = new List<char>();
            vertList.AddRange(this.Vert);

            var fragList = new List<char>();
            fragList.AddRange(this.Frag);

            UpdateNew("Exporting To: " + exportFile, 0, 1);

            this.Writer.Open();

            var cancellationToken = this.Token;
            this.Writer.WriteHeader(ref cancellationToken);

            UpdateNew("Writing VertexShader...", 1, 100);

            this.Writer.WriteList(vertList, ref cancellationToken);

            cancellationToken.ThrowIfCancellationRequested();

            UpdateNew("Writing FragmentShader...", 1, 100);
            this.Writer.WriteList(fragList, ref cancellationToken);

            this.Writer.Close();

            UpdateNew("Finished !", 1, 1);

            return true;
        }

        #endregion

        #region Import

        private bool ImportShaders(string vertexShader, string fragmentShader)
        {
            if (!File.Exists(vertexShader) || !File.Exists(fragmentShader))
                throw new ArgumentException("File Dose Not Exists");

            UpdateNew("Compiling...", 0, 1);

            var task = new Process {
                StartInfo = {
                    Arguments              = $"--vert={vertexShader} --frag={fragmentShader} --validate --err-format=glslang --lang=glsl",
                    ErrorDialog            = false,
                    CreateNoWindow         = true,
                    RedirectStandardError  = true,
                    RedirectStandardOutput = true,
                    FileName               = "glslcc.exe",
                },
            };

            task.Start();

            UpdateNew("  ------------------ BEGIN compile ------------------  ", 1, 1);

            while (!task.StandardOutput.EndOfStream)
            {
                UpdateNew(task.StandardOutput.ReadLine(), 1, 2);
            }

            while (!task.StandardError.EndOfStream)
            {
                UpdateNew(task.StandardError.ReadLine(), 1, 2);
            }

            UpdateNew("  ------------------  END compile  ------------------  ", 1, 1);

            UpdateNew($"Importing {vertexShader}...", 0, 1);
            this.Vert = ReplaceAllUnused(File.ReadAllText(vertexShader));
            UpdateNew($"Importing {fragmentShader}...", 0, 1);
            this.Frag = ReplaceAllUnused(File.ReadAllText(fragmentShader));

            UpdateNew("Finished Import!", 1, 1);
            return true;
        }

        private string ReplaceAllUnused(string readAllText)
        {
            return this.regexEmptyLine.Replace(this.replaceAllUnused.Replace(readAllText, ""),"");
        }

        private readonly Regex regexEmptyLine = new Regex("^(?:[\t ]*(?:\r?\n|\r))+", RegexOptions.Multiline);
        private readonly Regex replaceAllUnused = new Regex("^[ \t\\r\\n]*(//.*)*", RegexOptions.Multiline);
       
        public string Vert { get; set; }

        public string Frag { get; set; }

        #endregion
    }

    public class ShaderDescription
    {
        public string Name { get; set; }
    }
}
