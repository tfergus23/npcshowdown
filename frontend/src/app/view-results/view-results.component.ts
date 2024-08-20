import { Component, ViewChild } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { BattleService } from '../battle.service';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';
import { BattleLogViewComponent } from '../battle-log-view/battle-log-view.component';

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

  constructor(private route: ActivatedRoute, private battleService: BattleService, private dataService: DataService){
    this.tournamentID = Number(route.snapshot.paramMap.get('id'));
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
    });

    // Using two arrow functions here because apparently functions have their own 'this'
    this.interval = window.setInterval(() => {
      this.battleService.getTournamentResults(this.tournamentID).subscribe(
        (response) => {
          if (response.success){
            this.results = response.data;
            clearInterval(this.interval);
          }
          else{
            this.errorMessage = response.message;
          }
        },
        (error) => {
          this.errorMessage = error.error.message;
        });
    }, 1500);
    this.battleService.getTournamentResults(this.tournamentID).subscribe(
      (response) => {
        if (response.success){
          this.results = response.data;
          clearInterval(this.interval);
        }
        else{
          this.errorMessage = response.message;
        }
      },
      (error) => {
        this.errorMessage = error.error.message;
    });
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
}
