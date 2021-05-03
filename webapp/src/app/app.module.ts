import {NgModule} from '@angular/core';
import {BrowserModule} from '@angular/platform-browser';

import {AppRoutingModule} from './app-routing.module';
import {AppComponent} from './app.component';
import {DashboardComponent} from './dashboard/dashboard.component';
import {CubeComponent} from './cube/cube.component';
import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import {MatIconModule} from '@angular/material/icon';
import {MatToolbarModule} from '@angular/material/toolbar';
import {MatButtonModule} from '@angular/material/button';
import {MatTableModule} from '@angular/material/table';
import {SensorValuesComponent} from './sensor-values/sensor-values.component';
import {MatCardModule} from '@angular/material/card';
import {MatListModule} from '@angular/material/list';
import {MatGridListModule} from '@angular/material/grid-list';

@NgModule({
    declarations: [
        AppComponent,
        DashboardComponent,
        CubeComponent,
        SensorValuesComponent
    ],
    imports: [
        BrowserModule,
        AppRoutingModule,
        BrowserAnimationsModule,
        MatIconModule,
        MatToolbarModule,
        MatButtonModule,
        MatTableModule,
        MatCardModule,
        MatListModule,
        MatGridListModule
    ],
    providers: [],
    bootstrap: [AppComponent]
})
export class AppModule { }
