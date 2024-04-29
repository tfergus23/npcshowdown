import { Component, ElementRef, Input, ViewChild, ViewContainerRef } from '@angular/core';
import { CreateBattleComponent } from '../create-battle/create-battle.component';
import DataLists from 'src/DataLists';
import { CreatePokemonComponent } from '../create-pokemon/create-pokemon.component';
import Trainer from 'src/Trainer';
import Pokemon from 'src/Pokemon';

@Component({
  selector: 'app-create-trainer',
  templateUrl: './create-trainer.component.html',
  styleUrls: ['./create-trainer.component.css']
})
export class CreateTrainerComponent {
  collapsed: boolean;
  @Input() dataLists: DataLists = new DataLists();
  @Input() trainerNum: string = "1";
  @ViewChild('container', {read: ViewContainerRef}) container!: ViewContainerRef;
  @ViewChild('trainerInfo') trainerInfo!: ElementRef<HTMLElement>;
  pokeComponents: Array<CreatePokemonComponent> = new Array<CreatePokemonComponent>();
  nameInput?: HTMLInputElement;
  levelSelect?: HTMLSelectElement;
  constructor(){
    this.collapsed = false;
  }

  public toggleCollapsible() {
    this.collapsed = !this.collapsed;
  }

  ngAfterViewInit(){
    this.nameInput = (this.trainerInfo.nativeElement.querySelector('.trainer-name input') as HTMLInputElement);
    this.levelSelect = (this.trainerInfo.nativeElement.querySelector('.trainer-level select') as HTMLSelectElement);
    this.nameInput.value = this.trainerNum == '1' ? "Youngster Joey" : "Picknicker Heidi";
    this.levelSelect.value = "Wild";
  }

  addPoke(){
    let componentRef = this.container.createComponent(CreatePokemonComponent);
    let pokemonComponent = componentRef.instance;
    pokemonComponent.dataLists = this.dataLists;
    pokemonComponent.pokeNumber = (this.pokeComponents.length + 1).toString();
    pokemonComponent.parent = this;
    this.pokeComponents.push(pokemonComponent);
  }

  removePoke(pokeNumber: string){
    let index: number = parseInt(pokeNumber) - 1;
    this.container.remove(index);
    this.pokeComponents.splice(index,1);
    for(let i = 0; i < this.pokeComponents.length; i++){
      this.pokeComponents[i].pokeNumber = (i + 1).toString();
    }
  }

  getJSON() : Trainer{
    const name: string = this.nameInput!.value.trim();
    const level: string = this.levelSelect!.value.trim();
    let pokemon = new Array<Pokemon>();
    for(let i = 0; i < this.pokeComponents.length; i++){
      pokemon.push(this.pokeComponents[i].getJSON());
    }
    return {
      name: name,
      trainerLevel: level,
      team: pokemon
    };
  }

  setFromJson(json: Trainer){
    this.nameInput!.value = json.name;
    this.levelSelect!.value = json.trainerLevel;
    const pokesToAdd = json.team.length - this.pokeComponents.length;
    const pokesToRemove = this.pokeComponents.length - json.team.length;
    for (let i = 0; i < pokesToAdd; i++){
      this.addPoke();
    }
    for(let i = 0; i < pokesToRemove; i++){
      this.removePoke((this.pokeComponents.length).toString());
    }
    for (let i = 0; i < json.team.length; i++){
      //Calling setFromJSON here caused errors because the component isn't initialized immediately after calling addPoke
      this.pokeComponents[i].setFromJSON(json.team[i]);
    }
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

}
