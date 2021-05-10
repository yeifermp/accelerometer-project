import { AfterViewInit, ElementRef, OnDestroy, ViewChild } from '@angular/core';
import { Component } from '@angular/core';
import { BoxGeometry, Color, Mesh, MeshBasicMaterial, PerspectiveCamera, Scene, WebGLRenderer } from 'three';

@Component({
    selector: 'app-cube',
    templateUrl: './cube.component.html',
    styleUrls: ['./cube.component.css']
})
export class CubeComponent implements OnDestroy, AfterViewInit {
    private scene = new Scene();
    private camera!: PerspectiveCamera;
    private renderer = new WebGLRenderer({ antialias: true });
    private cube!: Mesh;
    private material = new MeshBasicMaterial({ color: 0x7b1fa2 });
    private geometry = new BoxGeometry();
    private xValue = 0;
    private yValue = 0;
    private accelX = 0;
    private accelY = 0;
    private accelZ = 0;
    private cubeWidth = 500;                        // Cube width
    private cubeHeight = 500;                       // Cube height
    private scaleFactorAccelerometer = 16384.0;     // Scale factor

    @ViewChild('cubeContainer') cubeContainer!: ElementRef;

    constructor() {

    }
    ngAfterViewInit(): void {
        if (this.cubeContainer) {
            const backgroundColor = new Color();
            this.camera = new PerspectiveCamera(20, this.cubeWidth / this.cubeHeight, 0.1, 1000);
            this.cube = new Mesh(this.geometry, this.material);
            this.renderer.setSize(this.cubeWidth, this.cubeHeight);
            this.scene.background = backgroundColor;
            this.scene.add(this.cube);
            this.camera.position.z = 5;

            this.cubeContainer.nativeElement.appendChild(this.renderer.domElement);

            const animate = () => {
                requestAnimationFrame(animate);
                this.renderer.render(this.scene, this.camera);

                // Calculate X axis angle in radians
                this.xValue = Math.atan(this.accelX / Math.sqrt(Math.pow(this.accelY, 2) + Math.pow(this.accelZ, 2)));

                //  Calculate Y axis angle in radians
                this.yValue = Math.atan(this.accelY / Math.sqrt(Math.pow(this.accelX, 2) + Math.pow(this.accelZ, 2)));

                this.cube.rotation.x = this.xValue;     // set rotation in X axis
                this.cube.rotation.y = this.yValue;     // set rotation in Y axis
            };

            animate();
        }
    }

    ngOnDestroy(): void {
        this.cubeContainer.nativeElement.removeChild(this.renderer.domElement);
    }

    setAccelX(value: number): void {
        // Get the accelerometer value and scale it appropriately
        this.accelX = value * this.scaleFactorAccelerometer;
    }

    setAccelY(value: number): void {
        // Get the accelerometer value and scale it appropriately
        this.accelY = value * this.scaleFactorAccelerometer;
    }

    setAccelZ(value: number): void {
        // Get the accelerometer value and scale it appropriately
        this.accelZ = value * this.scaleFactorAccelerometer;
    }
}
