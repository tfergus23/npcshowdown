import { Component, ViewChild } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { BattleService } from '../battle.service';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';
import { BattleLogViewComponent } from '../battle-log-view/battle-log-view.component';
import { AppComponent } from '../app.component';
import { UserService } from '../user.service';
import Trainer from 'src/Trainer';

const SORT_ELO = 0;
const SORT_WINS = 1;
const SORT_LOSSES = 2;
const SORT_RATIO = 3;
const SORT_BEST_WIN = 4;

@Component({
  selector: 'app-view-results',
  templateUrl: './view-results.component.html',
  styleUrls: ['./view-results.component.css']
})
export class ViewResultsComponent {
  tournamentID: number;
  results!: TournamentResultSet;
  dataLists!: DataLists;
  mostRecentSort: number = -1;
  errorMessage: string = "";
  interval: number = 0;
  @ViewChild('logView') logView!: BattleLogViewComponent;
  userTournaments: Array<TournamentResultSet> | undefined;

  constructor(private route: ActivatedRoute, private battleService: BattleService, private dataService: DataService, public app: AppComponent, private userService: UserService){
    this.tournamentID = Number(route.snapshot.paramMap.get('id'));
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
    });

    if (localStorage.getItem('user') != null)
    this.userService.getUserTournaments(localStorage.getItem('user') as string).subscribe((res) => {
      if (res.success){
        this.userTournaments = res.data;
      }
    }, 
    (error) =>{
      this.app.logoutUser();
      console.error(error);
    });

    // Using two arrow functions here because apparently functions have their own 'this'
    this.interval = window.setInterval(() => {
      this.battleService.getTournamentResults(this.tournamentID).subscribe(
        (response) => {
          if (response.success){
            this.results = response.data;
            this.results.trainers = new Array<Trainer>();
            clearInterval(this.interval);
            for (let i = 0; i < this.results.results.length; i++){
              this.battleService.getTournamentTrainer(this.results.results[i].id).subscribe((res)=>{
                this.results.trainers[this.results.results[i].index] = res.data;
              })
            }
          }
          else{
            this.errorMessage = response.message;
          }
        },
        (error) => {
          if (!error.error.success){
            clearInterval(this.interval);
          }
          this.errorMessage = error.error.message;
        });
    }, 1500);
    this.battleService.getTournamentResults(this.tournamentID).subscribe(
      (response) => {
        if (response.success){
          this.results = response.data;
          this.results.trainers = new Array<Trainer>();
          clearInterval(this.interval);
          for (let i = 0; i < this.results.results.length; i++){
            this.battleService.getTournamentTrainer(this.results.results[i].id).subscribe((res)=>{
              this.results.trainers[this.results.results[i].index] = res.data;
            })
          }
        }
        else{
          this.errorMessage = response.message;
        }
      },
      (error) => {
        if (!error.error.success){
          clearInterval(this.interval);
        }
        this.errorMessage = error.error.message;
    });
  }

  userHasTournament() : boolean{
    for (let i = 0; i < this.userTournaments!.length; i++){
      if (this.results.id == this.userTournaments![i].id){
        return true;
      }
    }
    return false;
  }

  sortByELO(){
    this.results.results.sort((r1,r2) =>{
      return r2.elo - r1.elo;
    });
  }

  sortByWins(){
    this.results.results.sort((r1,r2) =>{
      return r2.wins - r1.wins;
    });
  }

  sortByLosses(){
    this.results.results.sort((r1,r2) =>{
      return r2.losses - r1.losses;
    });
  }

  sortByWLRatio(){
    this.results.results.sort((r1,r2) =>{
      const ratio1 = r1.wins / r1.losses;
      const ratio2 = r2.wins / r2.losses;
      return ratio2 - ratio1;
    });
  }

  sortByBestWin(){
    this.results.results.sort((r1,r2) =>{
      return r2.bestWinEloDiff - r1.bestWinEloDiff;
    });
  }

  sortByName(){
    this.results.results.sort((r1,r2) =>{
      const name1 = this.results.trainers[r1.index].name;
      const name2 = this.results.trainers[r2.index].name;

      if (name1 > name2){
        return 1;
      }
      else{
        return -1;
      }
    });
  }

  sortByPokemonCount(){
    this.results.results.sort((r1,r2) =>{
      const count1 = this.results.trainers[r1.index].team.length;
      const count2 = this.results.trainers[r2.index].team.length;

      return count2 - count1;
    });
  }

  sortByTrainerLevel(){
    this.results.results.sort((r1,r2) =>{
      const level1 = this.results.trainers[r1.index].trainerLevel;
      const level2 = this.results.trainers[r2.index].trainerLevel;

      if (level1 > level2){
        return 1;
      }
      else{
        return -1;
      }
    });
  }

  sortByTrainerNumber(){
    this.results.results.sort((r1,r2) =>{
      return r1.index - r2.index;
    });
  }

  addTournamentToUser(){
    this.userService.addTournamentToUserProfile(this.app.loggedInUser!.name, this.results.id).subscribe((res) =>{
      this.app.setUserData(this.app.loggedInUser!.name);
    },
    (error) =>{
      if (error.status == 401){
        this.app.logoutUser();
      }
      console.error(error);
    });
  }
    download(){
      const json = this.results;
      const newBlob = new Blob([JSON.stringify(json, null, 4)], {
        type: 'application/json'
      });
      const data = window.URL.createObjectURL(newBlob);
      const link = document.createElement("a");
      link.href = data;
      link.download = `tournament${this.results.id}.json`; 
      link.click();
      link.remove();
    }
}
