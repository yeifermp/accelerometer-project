using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Connecting;
using MQTTnet.Client.Disconnecting;
using MQTTnet.Client.Options;
using TelemetricSystems.Api.Config;

namespace TelemetricSystems.Api.Services 
{
    public class MQTTClient : IHostedService
    {
        private readonly ILogger<MQTTClient> _logger;
        private readonly MQTTConfig _mqttConfig;
        private readonly IHubContext<AccelerometerHub, IAccelerometerHub> _accelerometerHubContext;

        public MQTTClient(IOptions<MQTTConfig> mqttConfigOptions, ILogger<MQTTClient> logger, 
            IHubContext<AccelerometerHub, IAccelerometerHub> accelerometerHubContext)
        {
            _accelerometerHubContext = accelerometerHubContext;
            _mqttConfig = mqttConfigOptions.Value;
            _logger = logger;
        }

        private IMqttClient _mqttClient;

        public async Task StartAsync(CancellationToken cancellationToken)
        {
            _logger.LogInformation("Creating MQTT client...");
            var factory = new MqttFactory();
            _mqttClient = factory.CreateMqttClient();

            try
            {
                var result = await _mqttClient.ConnectAsync(GetMqttClientOptions(), CancellationToken.None);

                if(result.ResultCode != MqttClientConnectResultCode.Success) 
                {
                    _logger.LogError($"Reason: {result.ReasonString}");
                    _logger.LogError($"ResponseInformation: {result.ResponseInformation}");
                } 
                else 
                {
                    _logger.LogInformation($"ResultCode: {result.ResultCode}");
                    await SubscribeToDefaultTopics();
                }      

                _mqttClient.UseDisconnectedHandler(DisconnectedAsync);
                _mqttClient.UseConnectedHandler(ConnectedAsync);
                _mqttClient.UseApplicationMessageReceivedHandler(MessageReceived);

                _logger.LogInformation("MQTT client has been created sucessfully.");
            }
            catch (Exception ex)
            {
                _logger.LogError($"Error: ${ex.Message}"); 
                throw;
            }
        }

        private IMqttClientOptions GetMqttClientOptions() 
        {
            _logger.LogInformation("Gathering MQTT options...");

            try
            {
                var currentPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                var caCertFileName = Path.Combine(currentPath, "Certificates/ca.crt");
                var clientCertFileName = Path.Combine(currentPath, "Certificates/api.pfx");
                _logger.LogInformation($"CA certificate: {caCertFileName}");
                _logger.LogInformation($"Client certificate: {clientCertFileName}");
                var caCert = X509Certificate.CreateFromCertFile(caCertFileName);
                var clientCert = new X509Certificate2(clientCertFileName, _mqttConfig.CertificatePassword);
                var tlsParameters = new MqttClientOptionsBuilderTlsParameters()
                {
                    UseTls = true,
                    SslProtocol = System.Security.Authentication.SslProtocols.Tls12,
                    Certificates = new List<X509Certificate>()
                    {
                        caCert, clientCert
                    },
                    CertificateValidationHandler = CertificateValidation
                };

                var options = new MqttClientOptionsBuilder()
                    .WithClientId($"Web_API_{Guid.NewGuid()}")
                    .WithTcpServer(_mqttConfig.Server, _mqttConfig.Port)
                    .WithTls(tlsParameters)
                    .Build();

                _logger.LogInformation("MQTT options gathered.");

                return options;
            }
            catch (Exception ex)
            {       
                _logger.LogError($"Error: ${ex.Message}");          
                throw;
            }
        }

        private bool CertificateValidation(MqttClientCertificateValidationCallbackContext args) 
        {
            // TODO: Use a real certificate in production and do not do this.
            return true;
        }

        private Task MessageReceived(MqttApplicationMessageReceivedEventArgs args) 
        {
            var payload = Encoding.UTF8.GetString(args.ApplicationMessage.Payload);

            if (args.ApplicationMessage.Topic == _mqttConfig.Topics.GyroscopeXTopicName) 
            {
                _accelerometerHubContext.Clients.All.SendGyroscopeX(float.Parse(payload));                
            }
            else if (args.ApplicationMessage.Topic == _mqttConfig.Topics.GyroscopeYTopicName) 
            {
                _accelerometerHubContext.Clients.All.SendGyroscopeY(float.Parse(payload));    
            }
            else if (args.ApplicationMessage.Topic == _mqttConfig.Topics.GyroscopeZTopicName) 
            {
                _accelerometerHubContext.Clients.All.SendGyroscopeZ(float.Parse(payload));    
            }
            else if (args.ApplicationMessage.Topic == _mqttConfig.Topics.AccelerometerXTopicName) 
            {
                _accelerometerHubContext.Clients.All.SendAccelerometerX(float.Parse(payload));    
            }
            else if (args.ApplicationMessage.Topic == _mqttConfig.Topics.AccelerometerYTopicName) 
            {
                _accelerometerHubContext.Clients.All.SendAccelerometerY(float.Parse(payload));    
            }
            else if (args.ApplicationMessage.Topic == _mqttConfig.Topics.AccelerometerZTopicName) 
            {
                _accelerometerHubContext.Clients.All.SendAccelerometerZ(float.Parse(payload));    
            }
            else if (args.ApplicationMessage.Topic == _mqttConfig.Topics.TemperatureTopicName) 
            {
                _accelerometerHubContext.Clients.All.SendTemperature(float.Parse(payload));    
            }
            
            _logger.LogInformation($"Topic: {args.ApplicationMessage.Topic}, Payload: {payload}");
            return Task.CompletedTask;
        }

        private async Task ConnectedAsync(MqttClientConnectedEventArgs arg) 
        {
            await SubscribeToDefaultTopics();
        }


        private async Task SubscribeToDefaultTopics() 
        {
            var topicBuilder = new MqttTopicFilterBuilder();            
            await _mqttClient.SubscribeAsync(_mqttConfig.Topics.AccelerometerXTopicName);
            await _mqttClient.SubscribeAsync(_mqttConfig.Topics.AccelerometerYTopicName);
            await _mqttClient.SubscribeAsync(_mqttConfig.Topics.AccelerometerZTopicName);
            await _mqttClient.SubscribeAsync(_mqttConfig.Topics.GyroscopeXTopicName);
            await _mqttClient.SubscribeAsync(_mqttConfig.Topics.GyroscopeYTopicName);
            await _mqttClient.SubscribeAsync(_mqttConfig.Topics.GyroscopeZTopicName);
            await _mqttClient.SubscribeAsync(_mqttConfig.Topics.TemperatureTopicName);
        }

        private async Task DisconnectedAsync(MqttClientDisconnectedEventArgs args) 
        {
            _logger.LogInformation("### DISCONNECTED FROM SERVER ###");
            _logger.LogInformation($"Reason: {args.Reason}");
            _logger.LogInformation($"ClientWasConnected: {args.ClientWasConnected}");
            _logger.LogInformation($"Reason: {args.Reason}");
            await Task.Delay(TimeSpan.FromSeconds(5));            

            try
            {
                await _mqttClient.ConnectAsync(GetMqttClientOptions(), CancellationToken.None);
            }
            catch
            {
                _logger.LogError("### RECONNECTING FAILED ###");
            }
        }

        public async Task StopAsync(CancellationToken cancellationToken)
        {
            await _mqttClient.DisconnectAsync();
        }
    }
}