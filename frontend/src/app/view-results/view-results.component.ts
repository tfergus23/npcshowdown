import { Component } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { BattleService } from '../battle.service';

@Component({
  selector: 'app-view-results',
  templateUrl: './view-results.component.html',
  styleUrls: ['./view-results.component.css']
})
export class ViewResultsComponent {
  tournamentID: number;
  results: string = "";

  constructor(private route: ActivatedRoute, private battleService: BattleService){
    this.tournamentID = Number(route.snapshot.paramMap.get('id'));
    this.battleService.getTournamentResults(this.tournamentID).subscribe(
    (response) => {
      if (response.success){
        this.results = JSON.stringify(response.data, null, 4);
      }
    },
    (error) => {
      console.log(error);
    });
  }
}
