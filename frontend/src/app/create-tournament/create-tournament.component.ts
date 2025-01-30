import { Component, ViewChild, ViewContainerRef } from '@angular/core';
import { AppComponent } from '../app.component';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { DataService } from '../data.service';
import DataLists from 'src/DataLists';
import Trainer, { createEmptyTrainer } from 'src/Trainer';
import TournamentRequest from 'src/TournamentRequest';
import { BattleService } from '../battle.service';

@Component({
  selector: 'app-create-tournament',
  templateUrl: './create-tournament.component.html',
  styleUrls: ['./create-tournament.component.css']
})
export class CreateTournamentComponent {
  @ViewChild('tournamentTrainerList', {read: ViewContainerRef}) trainerList!: ViewContainerRef;
  trainers: Array<Trainer> = new Array<Trainer>();
  dataLists!: DataLists;
  rounds: number = 25;
  seed: string = "";
  importingTrainers: boolean = false;

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
            console.log(error); // TODO: Show this to the user somehow.
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
    }, (error) =>{
      if (error.status == 401){
        request.user = undefined;
        this.app.loggedInUser = undefined;
        localStorage.removeItem('user');
        this.battleService.postTournamentRequest(request).subscribe((res) =>{
          this.redirectToTournamentResults(res.id);
        });
      }
      else{
        console.error(error);
      }
    });
  }
}
