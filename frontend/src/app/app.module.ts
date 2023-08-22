import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppComponent } from './app.component';
import { NavbarComponent } from './navbar/navbar.component';
import { AboutComponent } from './about/about.component';
import { MenuButtonComponent } from './menu-button/menu-button.component';
import { HttpClientModule } from '@angular/common/http';
import { ContentComponent } from './content/content.component';
import { HomeComponent } from './home/home.component';
import { AppRoutingModule } from './app-routing.module';
import { CreateBattleComponent } from './create-battle/create-battle.component';
import { CreateTournamentComponent } from './create-tournament/create-tournament.component';
import { ViewTrainersComponent } from './view-trainers/view-trainers.component';
import { ViewResultsComponent } from './view-results/view-results.component';

@NgModule({
  declarations: [
    AppComponent,
    NavbarComponent,
    AboutComponent,
    MenuButtonComponent,
    ContentComponent,
    HomeComponent,
    CreateBattleComponent,
    CreateTournamentComponent,
    ViewTrainersComponent,
    ViewResultsComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
