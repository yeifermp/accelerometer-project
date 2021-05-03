using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace TelemetricSystems.Api.Services
{
    public interface IAccelerometerHub 
    {
        Task SendAccelerometerX(double value);
        Task SendAccelerometerY(double value);
        Task SendAccelerometerZ(double value);
        Task SendGyroscopeX(double value);
        Task SendGyroscopeY(double value);
        Task SendGyroscopeZ(double value);
        Task SendTemperature(double value);
    }
}