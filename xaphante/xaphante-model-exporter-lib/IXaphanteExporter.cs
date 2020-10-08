using System;
using System.Threading;

namespace xaphante_model_exporter_lib
{
    public interface IXaphanteExporter
    {
        void Init(Action<ProgressEvent> statusCallback, CancellationToken token);
        
        public void InitTemplate(Action<ProgressEvent> statusCallback, CancellationToken token)
        {
            this.Last           = new ProgressEvent(0, "Initializing");
            this.StatusCallback = statusCallback;
            this.Token          = token;
        }

        ProgressEvent Last { get; set; }

        Action<ProgressEvent> StatusCallback { get; set; }
        CancellationToken     Token          { get; set; }

        FileWriter Writer { get; }

        public void UpdateLast(string msg);
        public void UpdateNew(string  msg, int now, int max);

        void UpdateLastTemplate(string msg)
        {
            this.StatusCallback?.Invoke(new ProgressEvent(this.Last.Percentage, msg));
        }

        void UpdateNewTemplate(string msg, int now, int max)
        {
            this.StatusCallback?.Invoke(new ProgressEvent(now / (double) max, msg));
        }
    }
}
