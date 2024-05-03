import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HomeComponent } from './home/home.component';
import { CreateBattleComponent } from './create-battle/create-battle.component';
import { CreateTournamentComponent } from './create-tournament/create-tournament.component';
import { ViewTrainersComponent } from './view-trainers/view-trainers.component';
import { ViewResultsComponent } from './view-results/view-results.component';

const routes: Routes = [
  {path: "", component: HomeComponent},
  {path: "battle", component: CreateBattleComponent},
  {path: "tournament", component: CreateTournamentComponent},
  {path: "trainers", component: ViewTrainersComponent},
  {path: "results/:id", component: ViewResultsComponent},
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }