import { Component } from '@angular/core';
import { Router } from '@angular/router';
import { BattleService } from '../battle.service';
import { AppComponent, MessageType } from '../app.component';
import { isResultSet, TournamentResultSet } from 'src/TournamentResultSet';

@Component({
  selector: 'app-results-lookup',
  templateUrl: './results-lookup.component.html',
  styleUrls: ['./results-lookup.component.css']
})
export class ResultsLookupComponent {
  results?: TournamentResultSet;
  constructor(public app: AppComponent, private router: Router, private battleService: BattleService){}

  searchForTournament(){
    const input = document.getElementById('tournament-lookup-search-btn') as HTMLInputElement;
    const value = input.value;
    if (!Number.isInteger(Number(value)) || Number(value) < 1){
      this.app.showMessage("Please enter a valid tournament ID.", MessageType.ERROR);
      return;
    }
    this.battleService.getTournamentResults(Number(value)).subscribe((res) =>{
      if (res.success){
        this.router.navigateByUrl(`/results/${value}`);
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) =>{
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
  }

  importFromFile(){
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
            alert("Invalid results file.");
          }
        }
        catch(error){
          console.log(error);
          alert("Invalid results file.");
        }

      })
    });
    input.click();
    input.remove();
  }
}
