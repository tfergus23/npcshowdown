import { Component, Input } from '@angular/core';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { UserTournamentsComponent } from '../user-tournaments/user-tournaments.component';

@Component({
  selector: 'app-results-info',
  templateUrl: './results-info.component.html',
  styleUrls: ['./results-info.component.css']
})
export class ResultsInfoComponent {
  @Input() resultSet!: TournamentResultSet;
  @Input() deleteable: boolean = false;
  @Input() parent: UserTournamentsComponent | undefined = undefined;

  openDeleteModal(id: number){
    this.parent?.openDeleteModal(id);
  }
}
