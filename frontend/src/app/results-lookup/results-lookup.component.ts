import { Component } from '@angular/core';
import { Router } from '@angular/router';
import { BattleService } from '../battle.service';
import { AppComponent, MessageType } from '../app.component';
import { isResultSet, TournamentResultSet } from 'src/TournamentResultSet';

interface RecentTournament {
  id: number;
  index: number;
}

@Component({
  selector: 'app-results-lookup',
  templateUrl: './results-lookup.component.html',
  styleUrls: ['./results-lookup.component.css']
})
export class ResultsLookupComponent {
  results?: TournamentResultSet;
  fetchingTournament: boolean = false;
  recentTournaments: Array<TournamentResultSet> = new Array<TournamentResultSet>();
  whileFetchingTournament = () => {return this.fetchingTournament};
  constructor(public app: AppComponent, private router: Router, private battleService: BattleService){}

  ngOnInit(){
    let storedRecents = localStorage.getItem('recent-tournaments');
    if (storedRecents != null){
      let recentIDs: Array<number> = JSON.parse(storedRecents);
      let recentTournaments: Array<RecentTournament> = new Array<RecentTournament>();
      recentIDs.forEach((id: number, index: number) => {
        recentTournaments.push({id: id, index: recentIDs.length - index - 1});
      });
      recentTournaments.forEach((recent) =>{
        this.battleService.getTournamentResults(recent.id).subscribe((res) =>{
          if (res.success){
            this.recentTournaments[recent.index] = res.data;
          }
        },
        (error) =>{

        });
      });
    }
  }

  searchForTournament = () =>{
    const input = document.getElementById('tournament-lookup-search-btn') as HTMLInputElement;
    const value = input.value;
    if (!Number.isInteger(Number(value)) || Number(value) < 1){
      this.app.showMessage("Please enter a valid tournament ID.", MessageType.ERROR);
      return;
    }
    this.fetchingTournament = true;
    this.battleService.getTournamentResults(Number(value)).subscribe((res) =>{
      this.fetchingTournament = false;
      if (res.success){
        this.router.navigateByUrl(`/results/${value}`);
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) =>{
      this.fetchingTournament = false;
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
  }

  importFromFile = () => {
    let input = document.createElement("input");
    input.type = 'file';
    input.addEventListener('change', (event) => {
      let file = input.files?.item(0);
      if (file != undefined && file.size > 512000){
        alert("File too big.");
        return;
      }
      file?.text().then(text => {
        try{
          let results: TournamentResultSet = JSON.parse(text) as TournamentResultSet;
          if (isResultSet(results)){
            this.results = results;
          }
          else{
            this.app.showMessage("Invalid results file.", MessageType.ERROR);
          }
        }
        catch(error){
          this.app.showMessage("Invalid results file.", MessageType.ERROR);
        }

      })
    });
    input.click();
    input.remove();
  }
}
