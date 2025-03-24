import { Component } from '@angular/core';
import { AppComponent } from '../app.component';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { UserService } from '../user.service';

@Component({
  selector: 'app-user-tournaments',
  templateUrl: './user-tournaments.component.html',
  styleUrls: ['./user-tournaments.component.css']
})
export class UserTournamentsComponent {
  resultSets: Array<TournamentResultSet> = new Array<TournamentResultSet>();
  constructor(public app: AppComponent, private userService: UserService){
    if (localStorage.getItem('user') != null)
      this.userService.getUserTournaments(localStorage.getItem('user') as string).subscribe((res) => {
        if (res.success){
          this.resultSets = res.data;
        }
      }, 
      (error) => {
        this.app.loggedInUser = undefined;
        localStorage.removeItem('user');
        console.error(error);
      });
  }
}
