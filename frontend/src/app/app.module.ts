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
import { CreateTrainerComponent } from './create-trainer/create-trainer.component';
import { CreatePokemonComponent } from './create-pokemon/create-pokemon.component';
import { FormsModule } from '@angular/forms';
import { DataListsComponent } from './data-lists/data-lists.component';
import { BattleLogViewComponent } from './battle-log-view/battle-log-view.component';
import { UserProfileComponent } from './user-profile/user-profile.component';
import { UserTrainersComponent } from './user-trainers/user-trainers.component';
import { UserTournamentsComponent } from './user-tournaments/user-tournaments.component';

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
    ViewResultsComponent,
    CreateTrainerComponent,
    CreatePokemonComponent,
    DataListsComponent,
    BattleLogViewComponent,
    UserProfileComponent,
    UserTrainersComponent,
    UserTournamentsComponent
  ],
  imports: [
    BrowserModule,
    HttpClientModule,
    AppRoutingModule,
    FormsModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
