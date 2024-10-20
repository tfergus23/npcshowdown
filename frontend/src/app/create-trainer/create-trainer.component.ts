import { Component, ElementRef, Input, ViewChild, ViewContainerRef } from '@angular/core';
import { CreateBattleComponent } from '../create-battle/create-battle.component';
import DataLists from 'src/DataLists';
import { CreatePokemonComponent } from '../create-pokemon/create-pokemon.component';
import Trainer, { createEmptyTrainer } from 'src/Trainer';
import Pokemon, { createEmptyPokemon } from 'src/Pokemon';
import { AppComponent } from '../app.component';
import { JsonpInterceptor } from '@angular/common/http';
import { CreateTournamentComponent } from '../create-tournament/create-tournament.component';
import { TournamentResult } from 'src/TournamentResultSet';
import { BattleService } from '../battle.service';
import { ViewResultsComponent } from '../view-results/view-results.component';

@Component({
  selector: 'app-create-trainer',
  templateUrl: './create-trainer.component.html',
  styleUrls: ['./create-trainer.component.css']
})
export class CreateTrainerComponent {
  @Input() collapsed: boolean = false;

  @Input() parent: CreateTournamentComponent  | undefined = undefined;
  @Input() dataLists?: DataLists = new DataLists();
  @Input() trainerNum: string = "1";
  @Input() trainer: Trainer = createEmptyTrainer();
  @Input() results: TournamentResult | undefined = undefined;
  @Input() readOnly: boolean = false;
  @Input() resultsView: ViewResultsComponent | undefined;

  clickedBestWin: boolean = false; // Dumb workaround for putting a button in a button
  selectedIndex: number = 0;

  constructor(public app: AppComponent, private battleService: BattleService){
    
  }

  public toggleCollapsible() {
    if (this.clickedBestWin){
      this.clickedBestWin = false;
      return;
    }
    this.collapsed = !this.collapsed;
  }

  ngAfterViewInit(){
  }

  addPoke(){
    this.trainer.team.push(createEmptyPokemon());
    this.selectedIndex = this.trainer.team.length-1;
  }

  removePoke(index: number){
    this.trainer.team.splice(index,1);
    if (this.selectedIndex == this.trainer.team.length){
      this.selectedIndex--;
    }
  }

  getJSON() : Trainer{
    return this.trainer;
  }

  setFromJson(json: Trainer){
    this.trainer = json;
  }

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

  download(){
    const json: Trainer = this.getJSON();
    const newBlob = new Blob([JSON.stringify(json, null, 4)], {
      type: 'application/json'
    });
    const data = window.URL.createObjectURL(newBlob);
    const link = document.createElement("a");
    link.href = data;
    link.download = `${json.name}.json`; 
    link.click();
    link.remove();
  }

  removeSelf(){
    this.parent?.removeTrainer(this.trainerNum);
  }

  showBattle(battleID: number){
    this.clickedBestWin = true;
    this.battleService.getBattle(battleID).subscribe((response) => {
      this.resultsView!.logView.battleID = battleID;
      this.resultsView!.logView.log = response.data;
      this.resultsView!.logView.hidden = false;
    });
  }

  setSelectedIndex(newIndex: number){
    this.selectedIndex = newIndex;
  }


  autoCompleteSpecies(event: FocusEvent){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].species = dataList.options[i].value;
        return;
      }
    }
    this.trainer.team[this.selectedIndex].species = "";
  }

  autoCompleteItem(event: FocusEvent){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].itemName = dataList.options[i].value;
        return;
      }
    }
    this.trainer.team[this.selectedIndex].itemName = "";
  }

  autoCompleteAbility(event: FocusEvent){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].abilityName = dataList.options[i].value;
        return;
      }
    }
    this.trainer.team[this.selectedIndex].abilityName = "";
  }

  autoCompleteMove(event: FocusEvent, index: number){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].moves[index] = dataList.options[i].value;
        return;
      }
    }
    this.trainer.team[this.selectedIndex].moves[index] = "";
  }
}
