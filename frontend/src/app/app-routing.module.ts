import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HomeComponent } from './home/home.component';
import { CreateBattleComponent } from './create-battle/create-battle.component';
import { CreateTournamentComponent } from './create-tournament/create-tournament.component';
import { ViewTrainersComponent } from './view-trainers/view-trainers.component';
import { ViewResultsComponent } from './view-results/view-results.component';
import { UserProfileComponent } from './user-profile/user-profile.component';
import { UserTrainersComponent } from './user-trainers/user-trainers.component';
import { UserTournamentsComponent } from './user-tournaments/user-tournaments.component';
import { SignUpComponent } from './sign-up/sign-up.component';
import { UserViewComponent } from './user-view/user-view.component';
import { ResultsLookupComponent } from './results-lookup/results-lookup.component';
import { AboutComponent } from './about/about.component';

const routes: Routes = [
  {path: "", component: HomeComponent},
  {path: "about", component: AboutComponent},
  {path: "battle", component: CreateBattleComponent},
  {path: "tournament", component: CreateTournamentComponent},
  {path: "trainers", component: ViewTrainersComponent},
  {path: "results", component: ResultsLookupComponent},
  {path: "results/:id", component: ViewResultsComponent},
  {path: "user/:username", component: UserViewComponent, children:[
    {path: "profile", component: UserProfileComponent},
    {path: "trainers", component: UserTrainersComponent},
    {path: "tournaments", component: UserTournamentsComponent},
  ]},
  {path: "signup", component: SignUpComponent},
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }