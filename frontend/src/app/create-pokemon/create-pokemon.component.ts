import { Component, ElementRef, Input, ViewChild } from '@angular/core';
import DataLists from 'src/DataLists';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { bootstrapApplication } from '@angular/platform-browser';
import Pokemon, { createEmptyPokemon } from 'src/Pokemon';
import { keyframes } from '@angular/animations';
import Trainer from 'src/Trainer';

@Component({
  selector: 'app-create-pokemon',
  templateUrl: './create-pokemon.component.html',
  styleUrls: ['./create-pokemon.component.css']
})
export class CreatePokemonComponent {
  @Input() dataLists?: DataLists = new DataLists();
  @Input() pokeNumber: string = "1";
  @Input() parent!: CreateTrainerComponent;
  @Input() trainer!: Trainer;
  @Input() pokemon!: Pokemon;
  @ViewChild('fields') fields!: ElementRef<HTMLElement>;
  collapsed: boolean = false;

  toggleCollapsible(){
    this.collapsed = !this.collapsed;
  }

  remove(){
    let index = parseInt(this.pokeNumber) - 1;
    this.trainer.team.splice(index, 1);
  }

  autoCompleteSpecies(event: FocusEvent){
    if (this.parent.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.pokemon.species = dataList.options[i].value;
        return;
      }
    }
    this.pokemon.species = "";
  }

  autoCompleteItem(event: FocusEvent){
    if (this.parent.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.pokemon.itemName = dataList.options[i].value;
        return;
      }
    }
    this.pokemon.itemName = "";
  }

  autoCompleteAbility(event: FocusEvent){
    if (this.parent.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.pokemon.abilityName = dataList.options[i].value;
        return;
      }
    }
    this.pokemon.abilityName = "";
  }

  autoCompleteMove(event: FocusEvent, index: number){
    if (this.parent.readOnly) return;
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        this.pokemon.moves[index] = dataList.options[i].value;
        return;
      }
    }
    this.pokemon.moves[index] = "";
  }

  setFromJSON(pokemon: Pokemon){
    this.pokemon = pokemon;
  }

  getJSON() : Pokemon{
    return {
      species: this.pokemon.species.trim(),
      nickname: this.pokemon.nickname.trim(),
      abilityName: this.pokemon.abilityName.trim(),
      itemName: this.pokemon.itemName.trim(),
      nature: this.pokemon.nature.trim(),
      gender: this.pokemon.gender.trim(),
      moves: [this.pokemon.moves[0].trim(),this.pokemon.moves[1].trim(),this.pokemon.moves[2].trim(),this.pokemon.moves[3].trim()],
      level: this.pokemon.level,
      evs: [this.pokemon.evs[0],this.pokemon.evs[1],this.pokemon.evs[2],this.pokemon.evs[3],this.pokemon.evs[4],this.pokemon.evs[5]],
      ivs: [this.pokemon.ivs[0],this.pokemon.ivs[1],this.pokemon.ivs[2],this.pokemon.ivs[3],this.pokemon.ivs[4],this.pokemon.ivs[5]],
    }
  }
}
