using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;
using Microsoft.Extensions.Logging;
using MQTTnet;
using MQTTnet.Server;
using Newtonsoft.Json;

namespace TelemetricSystems.Api.Services 
{
    public class MQTTRetainedMessageHandler : IMqttServerStorage
    {
        private readonly ILogger<MQTTRetainedMessageHandler> _logger; 
        public MQTTRetainedMessageHandler(ILogger<MQTTRetainedMessageHandler> logger)
        {
            _logger = logger;
        }

        private string _filename;
        public string Filename 
        {
            get 
            {
                if (string.IsNullOrEmpty(_filename)) 
                {
                    var currentPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                    _filename = Path.Combine(currentPath, "mqtt_server_data.json");
                    _logger.LogInformation($"Storing mqtt data in the following location: {_filename}");
                }

                return _filename;
            }
        }
        public Task<IList<MqttApplicationMessage>> LoadRetainedMessagesAsync()
        {
            _logger.LogInformation("Loading retained messages...");
            IList<MqttApplicationMessage> retainedMessages;
            if (File.Exists(Filename))
            {
                var json = File.ReadAllText(Filename);
                retainedMessages = JsonConvert.DeserializeObject<List<MqttApplicationMessage>>(json);
            }
            else
            {
                retainedMessages = new List<MqttApplicationMessage>();
            }
                
            _logger.LogInformation("Retained messages have been loaded successfully.");
            return Task.FromResult(retainedMessages);
        }

        public Task SaveRetainedMessagesAsync(IList<MqttApplicationMessage> messages)
        {
            _logger.LogInformation("Saaving retained messages...");
            File.WriteAllText(Filename, JsonConvert.SerializeObject(messages));
            _logger.LogInformation("Retained messages have been saved successfully.");
            return Task.FromResult(0);
        }
    }
}