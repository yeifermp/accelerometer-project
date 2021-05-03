using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Options;
using MQTTnet;
using MQTTnet.Server;
using TelemetricSystems.Api.Config;
using System.Security.Cryptography.X509Certificates;
using System.Security.Authentication;
using System.IO;
using System.Reflection;
using Microsoft.Extensions.Logging;
using System;

namespace TelemetricSystems.Api.Services 
{
    public class MQTTServer : IHostedService
    {
        private readonly MQTTConfig _mqttConfig;
        private IMqttServer _mqttServer;
        private readonly IMqttServerStorage _serverStorage;
        private readonly ILogger<MqttServer> _logger;
        public MQTTServer(IOptions<MQTTConfig> mqttConfigOptions, IMqttServerStorage serverStorage, ILogger<MqttServer> logger)
        {
            _mqttConfig = mqttConfigOptions.Value;
            _serverStorage = serverStorage;
            _logger = logger;
        }

        public async Task StartAsync(CancellationToken cancellationToken)
        {
            var factory = new MqttFactory();
            X509Certificate2 certificate = null;
            try 
            {
                var currentPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
                var fileName = Path.Combine(currentPath, "Certificate/certificate.pfx");
                _logger.LogInformation($"Obtaining the certificate from {fileName}...");
                
                certificate = new X509Certificate2(fileName, _mqttConfig.CertificatePassword, X509KeyStorageFlags.Exportable);
                _logger.LogInformation("The certificate has been obtained successfully.");
            }
            catch (Exception ex)
            {
                _logger.LogError($"Error trying to get the certificate. ${ex.Message}"); 
                throw;
            }
            

            var optionsBuilder = new MqttServerOptionsBuilder()
                .WithoutDefaultEndpoint()
                .WithEncryptedEndpoint()
                .WithEncryptedEndpointPort(_mqttConfig.Port)
                .WithStorage(_serverStorage)
                .WithEncryptionCertificate(certificate.Export(X509ContentType.Pfx))
                .WithEncryptionSslProtocol(SslProtocols.Tls12);

            _mqttServer = factory.CreateMqttServer();
                await _mqttServer.StartAsync(optionsBuilder.Build());
        }

        public async Task StopAsync(CancellationToken cancellationToken)
        {
            await _mqttServer.StopAsync();
        }
    }
}