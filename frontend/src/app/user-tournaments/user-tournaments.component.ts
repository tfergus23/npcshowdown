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
  showDeleteModal: boolean = false;
  tournamentToDelete: number = 0;
  constructor(public app: AppComponent, private userService: UserService){
    this.getUserTournaments();
  }

  getUserTournaments(){
    if (localStorage.getItem('user') != null)
      this.userService.getUserTournaments(localStorage.getItem('user') as string).subscribe((res) => {
        if (res.success){
          this.resultSets = res.data;
        }
      }, 
      (error) => {
        this.app.logoutUser();
        console.error(error);
      });
  }

  openDeleteModal(tournamentID: number){
    this.showDeleteModal = true;
    this.tournamentToDelete = tournamentID;
  }

  closeDeleteModal(){
    this.showDeleteModal = false;
    this.tournamentToDelete = 0;
  }

  deleteSelectedTournamentFromProfile(){
    this.userService.deleteTournamentFromUserProfile(this.app.loggedInUser!.name, this.tournamentToDelete).subscribe((res) =>{
      if (res.success){
        console.log("Tournament has been removed from your profile.");
        this.getUserTournaments();
      }
      else{
        console.error(res.message);
      }
    },
    (error) => {
      console.error(error);
    });
    this.closeDeleteModal();
  }
}
