import { Component } from '@angular/core';
import { AppComponent, MessageType } from '../app.component';
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
  tournamentToDelete: string = "0";
  fetchingTournaments: boolean = false;
  constructor(public app: AppComponent, private userService: UserService){
    this.getUserTournaments();
  }

  getUserTournaments(){
    if (localStorage.getItem('user') != null){
      this.fetchingTournaments = true;
      this.userService.getUserTournaments(localStorage.getItem('user') as string).subscribe((res) => {
        this.fetchingTournaments = false;
        if (res.success){
          this.resultSets = res.data;
        }
      }, 
      (error) => {
        this.fetchingTournaments = false;
        this.app.logoutUser();
        this.app.showMessage(error.error.message, MessageType.ERROR);
      });
    }

  }

  openDeleteModal(tournamentID: string){
    this.showDeleteModal = true;
    this.tournamentToDelete = tournamentID;
  }

  closeDeleteModal(){
    this.showDeleteModal = false;
    this.tournamentToDelete = "0";
  }

  deleteSelectedTournamentFromProfile(){
    this.userService.deleteTournamentFromUserProfile(this.app.loggedInUser!.name, this.tournamentToDelete).subscribe((res) =>{
      if (res.success){
        this.app.showMessage("Tournament has been removed from your profile.", MessageType.INFO);
        this.getUserTournaments();
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) => {
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
    this.closeDeleteModal();
  }
}
