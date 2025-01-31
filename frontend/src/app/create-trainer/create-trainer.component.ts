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
import { UserService } from '../user.service';

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

  selectedIndex: number = 0;
  collapseButtonSymbol: string = "▶";

  imageStrings: Array<string> = ["", "", "", "", "", ""];

  addTrainertext: string = "Add to My Trainers";
  trainerAdded: boolean = false;

  constructor(public app: AppComponent, private battleService: BattleService, private userService: UserService){
  }

  public toggleCollapsible() {
    this.collapsed = !this.collapsed;
    this.collapseButtonSymbol = (this.collapsed) ? "▶" : "▼";
  }

  ngOnInit(){
    this.setImagePathStrings();
  }

  ngAfterViewInit(){
    this.collapseButtonSymbol = (this.collapsed) ? "▶" : "▼";
  }

  addPoke(){
    this.trainer.team.push(createEmptyPokemon());
    this.selectedIndex = this.trainer.team.length-1;
    this.setImagePathStrings();
  }

  removePoke(index: number){
    this.trainer.team.splice(index,1);
    if (this.selectedIndex == this.trainer.team.length){
      this.selectedIndex--;
    }
    this.setImagePathStrings();
  }

  getJSON() : Trainer{
    return this.trainer;
  }

  setFromJson(json: Trainer){
    this.trainer = json;
    this.setImagePathStrings();
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
          this.selectedIndex = 0;
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

  showBattle(trainer1: Trainer, trainer2: Trainer, seed: string){
    this.battleService.postBattleRequest({trainer1, trainer2, seed}).subscribe((response) => {
      this.resultsView!.logView.battleID = 0;
      this.resultsView!.logView.log = response.data;
      this.resultsView!.logView.hidden = false;
    });
  }

  setSelectedIndex(newIndex: number){
    this.selectedIndex = newIndex;
  }

  setImagePathStrings(){
    for (let i = 0; i < this.trainer.team.length; i++){
      this.imageStrings[i] = this.trainer.team[i].species.toLowerCase();
    }
  }


  autoCompleteSpecies(event: FocusEvent){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].species = dataList.options[i].value;
        this.setImagePathStrings();
        return;
      }
    }
    this.trainer.team[this.selectedIndex].species = "";
    this.setImagePathStrings();
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

  saveToUserProfile(){
    if (this.app.loggedInUser)
    this.userService.addTrainerToUserProfile(this.app.loggedInUser.name, this.trainer).subscribe((res) => {
      console.log(`Success! Saved to ${res.id}`)
      this.addTrainertext = "Added ✓";
      this.trainerAdded = true;
    }, 
    (error) =>{
      if (error.status == 409){
        console.error(error.error.message);
      }
      else if (error.status == 401){
        this.app.loggedInUser = undefined;
        localStorage.removeItem('user');
      }
      else{
        console.error(error);
      }
    });
  }
}

