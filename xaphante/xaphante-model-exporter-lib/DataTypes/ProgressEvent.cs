namespace xaphante_model_exporter_lib
{
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
}