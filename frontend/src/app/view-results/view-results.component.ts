import { Component } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { BattleService } from '../battle.service';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';

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

  constructor(private route: ActivatedRoute, private battleService: BattleService, private dataService: DataService){
    this.tournamentID = Number(route.snapshot.paramMap.get('id'));
    this.battleService.getTournamentResults(this.tournamentID).subscribe(
    (response) => {
      if (response.success){
        this.results = response.data;
      }
    },
    (error) => {
      console.log(error);
    });
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
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
}
