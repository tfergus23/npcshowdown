import { Component, ViewChild, ViewContainerRef } from '@angular/core';
import { AppComponent, MessageType } from '../app.component';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { DataService } from '../data.service';
import DataLists from 'src/DataLists';
import Trainer, { createEmptyTrainer } from 'src/Trainer';
import TournamentRequest from 'src/TournamentRequest';
import { BattleService } from '../battle.service';
import { UserTrainersModalComponent } from '../user-trainers-modal/user-trainers-modal.component';

@Component({
  selector: 'app-create-tournament',
  templateUrl: './create-tournament.component.html',
  styleUrls: ['./create-tournament.component.css']
})
export class CreateTournamentComponent {
  @ViewChild('tournamentTrainerList', {read: ViewContainerRef}) trainerList!: ViewContainerRef;
  @ViewChild('trainerModal') trainerModal!: UserTrainersModalComponent;
  trainers: Array<Trainer> = new Array<Trainer>();
  dataLists!: DataLists;
  rounds: number = 25;
  seed: string = "";
  importingTrainers: boolean = false;
  errors: Array<string> = [];

  constructor(public app: AppComponent, private dataService: DataService, private battleService: BattleService){}

  ngOnInit() : void{
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
    });
  }

  addEmptyTrainer(){
    this.trainers.push(createEmptyTrainer());
  }

 removeTrainer(trainerNumber: string){
  let index: number = parseInt(trainerNumber) - 1;
  this.trainers.splice(index,1);
 }

  addFromFiles(){
    let input = document.createElement("input");
    input.type = 'file';
    input.multiple = true;
    input.addEventListener('change', (event) => {
      let files = input.files;
      if (!files || files.length <= 0){
        return;
      }
      this.importingTrainers = true;
      for(let i = 0; i < files.length; i++){
        let file = files.item(i);
        if (file?.size && file?.size > 30000){
          if (i+1 == files!.length){
            this.importingTrainers = false;
          }
          this.app.showMessage("One or more files were not able to be imported.", MessageType.ERROR);
          continue;
        }
        file?.text().then(text => {
          try{
            let trainer: Trainer = JSON.parse(text) as Trainer;
            this.trainers.push(trainer);
            if (i+1 == files!.length){
              this.importingTrainers = false;
            }
          }
          catch(error){
            this.app.showMessage("One or more files were not able to be imported.", MessageType.ERROR);
            if (i+1 == files!.length){
              this.importingTrainers = false;
            }
          }
        });

        
      }
    });
    input.click();
    input.remove();
  }

  numBattles(){
    let entrants = this.trainers.length;
    let matches = entrants * (entrants - 1) / 2;
    return matches * this.rounds;
  }

  redirectToTournamentResults(tournamentID: number){
    let a = document.createElement("a");
    a.href = `/results/${tournamentID}`;
    a.click();
    a.remove();
  }

  submitTournament(){
    let request: TournamentRequest = {
      trainers: this.trainers,
      seed: this.seed == "" ? Math.round((Math.random() * 2147483647)).toString() : this.seed,
      rounds: this.rounds,
      user: this.app.loggedInUser ? this.app.loggedInUser.name : undefined
    };
    this.battleService.postTournamentRequest(request).subscribe((res) =>{
      this.redirectToTournamentResults(res.id);
    }, 
    (error) =>{
      if (error.status == 401){
        request.user = undefined;
        this.app.showMessage("Session expired. Please log in again.", MessageType.ERROR);
        this.app.logoutUser();
        this.battleService.postTournamentRequest(request).subscribe((res) =>{
          this.redirectToTournamentResults(res.id);
        }, 
        (err) => {
          this.app.showMessage(err.error.message, MessageType.ERROR);
        });
      }
      else{
        this.errors = error.error.message.split("\n");
      }
    });
  }

  openTrainersModal(){
    this.trainerModal.show();
  }
}
