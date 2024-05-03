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

  constructor(public app: AppComponent, private dataService: DataService, private battleService: BattleService){}

  ngOnInit() : void{
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
    });
  }
/*
    let componentRef = this.container.createComponent(CreatePokemonComponent);
    let pokemonComponent = componentRef.instance;
    pokemonComponent.dataLists = this.dataLists;
    pokemonComponent.pokeNumber = (this.pokeComponents.length + 1).toString();
    pokemonComponent.parent = this;
    this.pokeComponents.push(pokemonComponent);
  */
  addEmptyTrainer(){
    this.trainers.push(createEmptyTrainer());
  }
  /*
    removePoke(pokeNumber: string){
    let index: number = parseInt(pokeNumber) - 1;
    this.container.remove(index);
    this.pokeComponents.splice(index,1);
    for(let i = 0; i < this.pokeComponents.length; i++){
      this.pokeComponents[i].pokeNumber = (i + 1).toString();
    }
  }
  */
 removeTrainer(trainerNumber: string){
  let index: number = parseInt(trainerNumber) - 1;
  this.trainers.splice(index,1);
 }
 /*
   loadFromFile(){
    let input = document.createElement("input");
    input.type = 'file';
    input.addEventListener('change', (event) => {
      let file = input.files?.item(0);
      if (file != undefined && file.size > 8192){
        alert("File too big.");
        return;
      }
      file?.text().then(text => {
        try{
          let trainer: Trainer = JSON.parse(text) as Trainer;
          this.setFromJson(trainer);
        }
        catch(error){
          console.log(error);
          alert("Invalid trainer file.");
        }

      })
    });
    input.click();
    input.remove();
  }
 */
  addFromFiles(){
    let input = document.createElement("input");
    input.type = 'file';
    input.multiple = true;
    input.addEventListener('change', (event) => {
      let files = input.files;
      if (!files || files.length <= 0){
        return;
      }
      for(let i = 0; i < files.length; i++){
        let file = files.item(i);
        file?.text().then(text => {
          try{
            let trainer: Trainer = JSON.parse(text) as Trainer;
            this.trainers.push(trainer);
          }
          catch(error){
            console.log(error); // TODO: Show this to the user somehow.
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

  submitTournament(){
    let request: TournamentRequest = {
      trainers: this.trainers,
      seed: this.seed == "" ? Math.round((Math.random() * 2147483647)).toString() : this.seed,
      rounds: this.rounds
    };
    this.battleService.postTournamentRequest(request).subscribe((res) =>{
      if (!res.success){
        console.log(res.message);
        return;
      }
      let a = document.createElement("a");
      a.href = `/results/${res.id}`;
      a.click();
      a.remove();
    });
  }
}
