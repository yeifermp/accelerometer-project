import { AfterViewInit, Component, OnDestroy, OnInit, ViewChild } from '@angular/core';
import { Subject } from 'rxjs';
import { map, takeUntil } from 'rxjs/operators';
import { AccelerometerHubService } from '../accelerometer-hub.service';
import { CubeComponent } from '../cube/cube.component';
import { SensorValuesComponent } from '../sensor-values/sensor-values.component';

@Component({
    selector: 'app-dashboard',
    templateUrl: './dashboard.component.html',
    styleUrls: ['./dashboard.component.css']
})
export class DashboardComponent implements OnInit, AfterViewInit, OnDestroy {
    @ViewChild(CubeComponent) cubeComponent!: CubeComponent;
    @ViewChild(SensorValuesComponent) sensorValuesComponent!: SensorValuesComponent;
    destroy$: Subject<boolean> = new Subject<boolean>();

    constructor(private accelerometerHub: AccelerometerHubService) {

    }
    ngOnDestroy(): void {
        this.destroy$.next(true);
        this.destroy$.unsubscribe();
    }

    ngAfterViewInit(): void {
        this.accelerometerHub.onAccelX.pipe(takeUntil(this.destroy$), map(value => {
            this.cubeComponent.setAccelX(value);
            this.sensorValuesComponent.setAccelX(value);
        })).subscribe();

        this.accelerometerHub.onAccelY.pipe(takeUntil(this.destroy$), map(value => {
            this.cubeComponent.setAccelY(value);
            this.sensorValuesComponent.setAccelY(value);
        })).subscribe();

        this.accelerometerHub.onAccelZ.pipe(takeUntil(this.destroy$), map(value => {
            this.cubeComponent.setAccelZ(value);
            this.sensorValuesComponent.setAccelZ(value);
        })).subscribe();

        this.accelerometerHub.onTemp.pipe(takeUntil(this.destroy$), map(value => this.sensorValuesComponent.setTemperature(value))).subscribe();
    }

    ngOnInit(): void {
    }
}
