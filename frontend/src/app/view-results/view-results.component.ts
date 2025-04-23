import { Component, Input, ViewChild } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { BattleService } from '../battle.service';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';
import { BattleLogViewComponent } from '../battle-log-view/battle-log-view.component';
import { AppComponent, MessageType } from '../app.component';
import { UserService } from '../user.service';
import Trainer from 'src/Trainer';
import GetResponse from 'src/GetResponse';

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
  tournamentID: string | null;
  @Input() results?: TournamentResultSet;
  dataLists!: DataLists;
  mostRecentSort: number = -1;
  errorMessage: string = "";
  interval: number = 0;
  @ViewChild('logView') logView!: BattleLogViewComponent;
  @Input() showSaveButtons: boolean = true;
  userTournaments: Array<TournamentResultSet> | undefined;
  addingToUser: boolean = false;
  whileAddingToUser = () => {return this.addingToUser;};

  constructor(private route: ActivatedRoute, private battleService: BattleService, private dataService: DataService, public app: AppComponent, private userService: UserService, private router: Router){
    this.tournamentID = route.snapshot.paramMap.get('id');
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
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });

    if (this.results) return;

    // Using two arrow functions here because apparently functions have their own 'this'
    this.interval = window.setInterval(() => {
      this.battleService.getTournamentResults(Number(this.tournamentID)).subscribe(
        (response) => {
          this.getTournamentSuccess(response);
        },
        (error) => {
          this.getTournamentError(error);
        });
    }, 1500);
    this.battleService.getTournamentResults(Number(this.tournamentID)).subscribe(
      (response) => {
        this.getTournamentSuccess(response);
      },
      (error) => {
        this.getTournamentError(error);
    });
  }

  ngOnDestroy(){
    clearInterval(this.interval);
  }

  userHasTournament() : boolean{
    for (let i = 0; i < this.userTournaments!.length; i++){
      if (this.results!.id == this.userTournaments![i].id){
        return true;
      }
    }
    return false;
  }

  sortByELO(){
    this.results!.results.sort((r1,r2) =>{
      return r2.elo - r1.elo;
    });
  }

  sortByWins(){
    this.results!.results.sort((r1,r2) =>{
      return r2.wins - r1.wins;
    });
  }

  sortByLosses(){
    this.results!.results.sort((r1,r2) =>{
      return r2.losses - r1.losses;
    });
  }

  sortByWLRatio(){
    this.results!.results.sort((r1,r2) =>{
      const ratio1 = r1.wins / r1.losses;
      const ratio2 = r2.wins / r2.losses;
      return ratio2 - ratio1;
    });
  }

  sortByBestWin(){
    this.results!.results.sort((r1,r2) =>{
      return r2.bestWinEloDiff - r1.bestWinEloDiff;
    });
  }

  sortByName(){
    this.results!.results.sort((r1,r2) =>{
      const name1 = this.results!.trainers[r1.index].name;
      const name2 = this.results!.trainers[r2.index].name;

      if (name1 > name2){
        return 1;
      }
      else{
        return -1;
      }
    });
  }

  sortByPokemonCount(){
    this.results!.results.sort((r1,r2) =>{
      const count1 = this.results!.trainers[r1.index].team.length;
      const count2 = this.results!.trainers[r2.index].team.length;

      return count2 - count1;
    });
  }

  sortByTrainerLevel(){
    this.results!.results.sort((r1,r2) =>{
      const level1 = this.results!.trainers[r1.index].trainerLevel;
      const level2 = this.results!.trainers[r2.index].trainerLevel;

      if (level1 > level2){
        return 1;
      }
      else{
        return -1;
      }
    });
  }

  sortByTrainerNumber(){
    this.results!.results.sort((r1,r2) =>{
      return r1.index - r2.index;
    });
  }

  addTournamentToUser = () => {
    this.addingToUser = true;
    this.userService.addTournamentToUserProfile(this.app.loggedInUser!.name, this.results!.id).subscribe((res) =>{
      if (res.success){
        this.app.setUserData(this.app.loggedInUser!.name);
        this.userService.getUserTournaments(localStorage.getItem('user') as string).subscribe((res) => {
          this.addingToUser = false;
          if (res.success){
            this.userTournaments = res.data;
          }
        }, 
        (error) =>{
          this.addingToUser = false;
          this.app.logoutUser();
          this.app.showMessage(error.error.message, MessageType.ERROR);
        });
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) =>{
      this.addingToUser = false;
      if (error.status == 401){
        this.app.logoutUser();
      }
      this.app.showMessage(error.error.message, MessageType.ERROR);
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
      link.download = `tournament${this.results!.id}.json`; 
      link.click();
      link.remove();
  }

  getTournamentSuccess(response: GetResponse){
    if (response.data.status == 'done'){
      this.results = response.data as TournamentResultSet;
      this.results.trainers = new Array<Trainer>();
      clearInterval(this.interval);
      for (let i = 0; i < this.results.results.length; i++){
        this.battleService.getTournamentTrainer(this.results.results[i].id).subscribe((res)=>{
          this.results!.trainers[this.results!.results[i].index] = res.data;
        })
      }
    }
    else{
      this.errorMessage = response.message;
    }
  }

  getTournamentError(error: any){
    if (!error.error.success){
      clearInterval(this.interval);
    }
    this.errorMessage = error.error.message;
  }

  saveTournamentText(){
    return this.userHasTournament() ? "Saved ✓" : "Add to My Tournaments";
  }

  addButtonBackground(){
    if (this.userHasTournament()){
      return 'rgb(0,255,0)';
    }
    else if (this.addingToUser){
      return 'gray';
    }
    else{
      return 'aqua';
    }
  }
}
