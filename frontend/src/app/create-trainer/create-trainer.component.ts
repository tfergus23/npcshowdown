import { BootstrapOptions, Component, ElementRef, Input, ViewChild, ViewContainerRef } from '@angular/core';
import { CreateBattleComponent } from '../create-battle/create-battle.component';
import DataLists from 'src/DataLists';
import { CreatePokemonComponent } from '../create-pokemon/create-pokemon.component';
import Trainer, { createEmptyTrainer } from 'src/Trainer';
import Pokemon, { createEmptyPokemon } from 'src/Pokemon';
import { AppComponent, MessageType } from '../app.component';
import { JsonpInterceptor } from '@angular/common/http';
import { CreateTournamentComponent } from '../create-tournament/create-tournament.component';
import { TournamentResult } from 'src/TournamentResultSet';
import { BattleService } from '../battle.service';
import { ViewResultsComponent } from '../view-results/view-results.component';
import { UserService } from '../user.service';
import { UserTrainersModalComponent } from '../user-trainers-modal/user-trainers-modal.component';
import { UserTrainersComponent } from '../user-trainers/user-trainers.component';
import { Router } from '@angular/router';

@Component({
  selector: 'app-create-trainer',
  templateUrl: './create-trainer.component.html',
  styleUrls: ['./create-trainer.component.css']
})
export class CreateTrainerComponent {
  @Input() collapsed: boolean = false;

  @Input() parent: CreateTournamentComponent  | UserTrainersComponent | undefined = undefined;
  @Input() dataLists?: DataLists = new DataLists();
  @Input() trainerNum: string = "1";
  @Input() trainer: Trainer = createEmptyTrainer();
  @Input() results: TournamentResult | undefined = undefined;
  @Input() readOnly: boolean = false;
  @Input() resultsView: ViewResultsComponent | undefined;
  @Input() showAddMyTrainers: boolean = true;
  @Input() showEditButton: boolean = false;
  @Input() onEdit: Function = () => {};

  editing: boolean = false;

  selectedIndex: number = 0;

  imageStrings: Array<string> = ["", "", "", "", "", ""];

  addTrainertext: string = "Add to My Trainers";

  @ViewChild('trainerModal') trainerModal!: UserTrainersModalComponent;

  oldTrainer: Trainer | undefined;

  showDeleteModal: boolean = false;
  addingToProfile: boolean = false;
  whileAddingToProfile = () => {return this.addingToProfile};

  constructor(public app: AppComponent, private battleService: BattleService, private userService: UserService, private router: Router){
  }

  public toggleCollapsible() {
    this.collapsed = !this.collapsed;
  }

  ngOnInit(){
    this.setImagePathStrings();
  }

  addPoke(){
    this.trainer.team.push(createEmptyPokemon());
    this.selectedIndex = this.trainer.team.length-1;
    this.setImagePathStrings();
    this.callOnEdit();
  }

  removePoke(index: number){
    this.trainer.team.splice(index,1);
    if (this.selectedIndex == this.trainer.team.length){
      this.selectedIndex--;
    }
    console.log(this.selectedIndex);
    this.setImagePathStrings();
    this.callOnEdit();
  }

  getJSON() : Trainer{
    return this.trainer;
  }

  setFromJson(json: Trainer){
    this.trainer.team = json.team;
    this.trainer.id = json.id;
    this.trainer.trainerLevel = json.trainerLevel;
    this.trainer.name = json.name;
    if (this.trainer.team.length > 0){
      this.selectedIndex = 0;
    }
    this.setImagePathStrings();
    this.callOnEdit();
  }

  loadFromFile(){
    let input = document.createElement("input");
    input.type = 'file';
    input.addEventListener('change', (event) => {
      let file = input.files?.item(0);
      if (file != undefined && file.size > 8192){
        this.app.showMessage("File too big.", MessageType.ERROR);
        return;
      }
      file?.text().then(text => {
        try{
          let trainer: Trainer = JSON.parse(text) as Trainer;
          this.setFromJson(trainer);
          this.selectedIndex = 0;
        }
        catch(error){
          this.app.showMessage("Invalid trainer file.", MessageType.ERROR);
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
    (this.parent as CreateTournamentComponent).removeTrainer(this.trainerNum);
  }

  showBattle(trainer1: Trainer, trainer2: Trainer, seed: string){
    const type = "text";
    if (!trainer1 || ! trainer2) return;
    let battleJson = {
      trainer1: trainer1,
      trainer2: trainer2,
      seed: seed,
      type: 'events'
    };
    this.router.navigate(['/battle-viewer'], {queryParams: {battle: encodeURIComponent(JSON.stringify(battleJson))}});
    this.battleService.postBattleRequest({trainer1, trainer2, seed, type}).subscribe((response) => {
      this.resultsView!.logView.log = response.data;
      this.resultsView!.logView.hidden = false;
    });
  }

  setSelectedIndex(newIndex: number){
    this.selectedIndex = newIndex;
  }

  setImagePathStrings(){
    for (let i = 0; i < this.trainer.team.length; i++){
      let string: string = this.trainer.team[i].species.toLowerCase();
      //Windows doesn't like : in file paths
      if (this.trainer.team[i].species == "Type: Null"){
        string = "type_null";
      }
      this.imageStrings[i] = string;
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
        this.callOnEdit();
        return;
      }
    }
    this.trainer.team[this.selectedIndex].species = "";
    this.setImagePathStrings();
    this.callOnEdit();
  }

  autoCompleteItem(event: FocusEvent){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].itemName = dataList.options[i].value;
        this.callOnEdit();
        return;
      }
    }
    this.trainer.team[this.selectedIndex].itemName = "";
    this.callOnEdit();
  }

  autoCompleteAbility(event: FocusEvent){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].abilityName = dataList.options[i].value;
        this.callOnEdit();
        return;
      }
    }
    this.trainer.team[this.selectedIndex].abilityName = "";
    this.callOnEdit();
  }

  autoCompleteMove(event: FocusEvent, index: number){
    if (this.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.trainer.team[this.selectedIndex].moves[index] = dataList.options[i].value;
        this.callOnEdit();
        return;
      }
    }
    this.trainer.team[this.selectedIndex].moves[index] = "";
    this.callOnEdit();
  }

  saveToUserProfile = () => {
    if (this.app.loggedInUser){
      this.addingToProfile = true;
      this.userService.addTrainerToUserProfile(this.app.loggedInUser.name, this.trainer).subscribe((res) => {
        this.addingToProfile = false;
        this.app.showMessage(`Success! Saved to ${res.id}`, MessageType.INFO);
      }, 
      (error) =>{
        this.addingToProfile = false;
        this.app.showMessage(error.error.message.split("\n")[0], MessageType.ERROR);
        if (error.status == 401){
          this.app.logoutUser();
        }
      });
    }
  }

  showTrainerModal(){
    this.trainerModal.show();
  }

  edit(){
    this.readOnly = false;
    this.editing = true;
    this.collapsed = false;
    this.oldTrainer = JSON.parse(JSON.stringify(this.trainer));
  }

  updateTrainer(){
    this.userService.updateUserTrainer(localStorage.getItem('user') as string, this.trainer).subscribe((res) =>{
      if (res.success){
        this.readOnly = true;
        this.editing = false;
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) => {
      if (error.status == 401){
        this.app.loggedInUser = undefined;
        localStorage.removeItem("user");
      }
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
  }

  revertChanges(){
    this.trainer = this.oldTrainer!;
    this.readOnly = true;
    this.editing = false;
    if (this.selectedIndex >= this.trainer.team.length){
      this.selectedIndex = this.trainer.team.length-1;
    }
    if (this.selectedIndex < 0){
      this.selectedIndex = 0;
    }
    this.setImagePathStrings();
  }

  openDeleteModal(){
    this.showDeleteModal = true;
  }

  closeDeleteModal(e: MouseEvent){
    this.showDeleteModal = false;
  }

  deleteFromProfile(){
    this.showDeleteModal = false;
    this.userService.deleteUserTrainer(this.app.loggedInUser!.name, this.trainer.id!).subscribe(
    (res) =>{
      this.app.showMessage("Trained deleted from your profile.", MessageType.INFO);
      (this.parent as UserTrainersComponent).trainers! = (this.parent as UserTrainersComponent).trainers!.filter(trainer => trainer.id != this.trainer.id);
    },
    (error)=>{
      this.app.showMessage(error.error.message, MessageType.ERROR);
    })
  }

  parentIsCreateTournament(){
    return this.parent instanceof CreateTournamentComponent;
  }

  callOnEdit(){
    if (!this.readOnly){
      console.log('onEdit');
      this.onEdit();
    } 
  }
}

