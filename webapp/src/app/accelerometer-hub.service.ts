import { EventEmitter, Injectable } from '@angular/core';
import * as signalR from '@microsoft/signalr';
import { environment } from 'src/environments/environment';

@Injectable({
    providedIn: 'root'
})
export class AccelerometerHubService {
    public onAccelX: EventEmitter<number> = new EventEmitter<number>();
    public onAccelY: EventEmitter<number> = new EventEmitter<number>();
    public onAccelZ: EventEmitter<number> = new EventEmitter<number>();
    public onGyroX: EventEmitter<number> = new EventEmitter<number>();
    public onGyroY: EventEmitter<number> = new EventEmitter<number>();
    public onGyroZ: EventEmitter<number> = new EventEmitter<number>();
    public onTemp: EventEmitter<number> = new EventEmitter<number>();

    constructor() {
        const connection = new signalR.HubConnectionBuilder()
            .configureLogging(signalR.LogLevel.Information)
            .withUrl(environment.apiUrl + 'accelerometer-notifier')
            .build();

        connection.start().then(() => {

        }).catch((err) => {
            return console.error(err);
        });

        connection.on('SendGyroscopeX', value => this.onGyroX.emit(value));
        connection.on('SendGyroscopeY', value => this.onGyroY.emit(value));
        connection.on('SendGyroscopeZ', value => this.onGyroZ.emit(value));
        connection.on('SendAccelerometerX', value => this.onAccelX.emit(value));
        connection.on('SendAccelerometerY', value => this.onAccelY.emit(value));
        connection.on('SendAccelerometerZ', value => this.onAccelZ.emit(value));
        connection.on('SendTemperature', value => this.onTemp.emit(value));
    }
}

