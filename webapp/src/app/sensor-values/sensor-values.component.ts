import { Component, OnInit } from '@angular/core';

@Component({
     selector: 'app-sensor-values',
     templateUrl: './sensor-values.component.html',
     styleUrls: ['./sensor-values.component.css']
})
export class SensorValuesComponent implements OnInit {
     dataSource = Array<IValueModel>();
     displayedColumns: string[] = ['position', 'name', 'value'];
     public gyroX = 0;
     public gyroY = 0;
     public gyroZ = 0;
     public accelX = 0;
     public accelY = 0;
     public accelZ = 0;
     public temperature = 0;
     public angleX = 0;
     public angleY = 0;

     constructor() {

     }

     ngOnInit(): void {

     }

     calculateAngles(): void {
          this.angleX = Math.atan(this.accelX / Math.sqrt(Math.pow(this.accelY, 2) + Math.pow(this.accelZ, 2)));
          this.angleY = Math.atan(this.accelY / Math.sqrt(Math.pow(this.accelX, 2) + Math.pow(this.accelZ, 2)));

          this.angleX = (this.angleX * 180) / Math.PI;
          this.angleY = (this.angleY * 180) / Math.PI;

          this.angleY = Math.round((this.angleY + Number.EPSILON) * 100) / 100;
          this.angleX = Math.round((this.angleX + Number.EPSILON) * 100) / 100;
     }

     setAccelX(value: number): void {
          this.accelX = Math.round((value + Number.EPSILON) * 100) / 100;
          this.calculateAngles();
     }

     setAccelY(value: number): void {
          this.accelY = Math.round((value + Number.EPSILON) * 100) / 100;
          this.calculateAngles();
     }

     setAccelZ(value: number): void {
          this.accelZ = Math.round((value + Number.EPSILON) * 100) / 100;
          this.calculateAngles();
     }

     setGyroX(value: number): void {
          this.gyroX = Math.round((value + Number.EPSILON) * 100) / 100;
     }

     setGyroY(value: number): void {
          this.gyroY = Math.round((value + Number.EPSILON) * 100) / 100;
     }

     setGyroZ(value: number): void {
          this.gyroZ = Math.round((value + Number.EPSILON) * 100) / 100;
     }

     setTemperature(value: number): void {
          this.temperature = Math.round((value + Number.EPSILON) * 100) / 100;
     }
}

export interface IValueModel {
     position: number;
     name: string;
     value: number;
}
