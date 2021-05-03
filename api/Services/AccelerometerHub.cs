using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace TelemetricSystems.Api.Services
{
    public class AccelerometerHub: Hub<IAccelerometerHub> 
    {
    }
}