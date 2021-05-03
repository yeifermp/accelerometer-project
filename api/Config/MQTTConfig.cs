namespace TelemetricSystems.Api.Config 
{
    public class MQTTConfig 
    {
        public TopicsConfig Topics { get; set; }
        public string CertificatePassword { get; set; }
        public int Port { get; set; }
        public string Server { get; set; }
    }

    public class TopicsConfig 
    {
        public string AccelerometerXTopicName { get; set; }
        public string AccelerometerYTopicName { get; set; }
        public string AccelerometerZTopicName { get; set; }
        public string GyroscopeXTopicName { get; set; }
        public string GyroscopeYTopicName { get; set; }
        public string GyroscopeZTopicName { get; set; }
        public string TemperatureTopicName { get; set; }
        
    }
}