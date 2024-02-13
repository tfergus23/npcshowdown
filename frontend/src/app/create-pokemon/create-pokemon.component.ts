import { Component, ElementRef, Input, ViewChild } from '@angular/core';
import DataLists from 'src/DataLists';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { bootstrapApplication } from '@angular/platform-browser';
import Pokemon from 'src/Pokemon';
import { keyframes } from '@angular/animations';

@Component({
  selector: 'app-create-pokemon',
  templateUrl: './create-pokemon.component.html',
  styleUrls: ['./create-pokemon.component.css']
})
export class CreatePokemonComponent {
  @Input() dataLists: DataLists = new DataLists();
  @Input() pokeNumber: string = "1";
  @Input() parent!: CreateTrainerComponent;
  @ViewChild('fields') fields!: ElementRef<HTMLElement>;
  collapsed: boolean = false;

  toggleCollapsible(){
    this.collapsed = !this.collapsed;
  }

  remove(){
    this.parent.removePoke(this.pokeNumber);
  }

  autoComplete(event: FocusEvent){
    let input: HTMLInputElement = event.target as HTMLInputElement;
    let dataList: HTMLDataListElement = input.list as HTMLDataListElement;
    for (let i = 0; i < dataList.options.length; i++){
      let inputVal = input.value.toLowerCase().trim();
      if (dataList.options[i].value.toLowerCase().includes(inputVal) && inputVal != ""){
        input.value = dataList.options[i].value;
        return;
      }
    }
    input.value = "";
  }

  getJSON() : Pokemon{
    const speciesInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-species input') as HTMLInputElement;
    const nicknameInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-nickname input') as HTMLInputElement;
    const abilityInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-ability input') as HTMLInputElement;
    const itemInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-item input') as HTMLInputElement;
    const natureSelect: HTMLSelectElement = this.fields.nativeElement.querySelector('.pokemon-nature select') as HTMLSelectElement;
    const genderSelect: HTMLSelectElement = this.fields.nativeElement.querySelector('.pokemon-gender select') as HTMLSelectElement;
    const moveInputs: NodeListOf<HTMLSelectElement> = this.fields.nativeElement.querySelectorAll('.pokemon-move input');
    const levelInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-level input') as HTMLInputElement;
    const valueInputs: NodeListOf<HTMLInputElement> = this.fields.nativeElement.querySelectorAll('.evs-ivs input');


    return {
      species: speciesInput.value.trim(),
      nickname: nicknameInput.value.trim(),
      abilityName: abilityInput.value.trim(),
      itemName: itemInput.value.trim(),
      nature: natureSelect.value.trim(),
      gender: genderSelect.value.trim(),
      moves: [moveInputs[0].value.trim(),moveInputs[1].value.trim(),moveInputs[2].value.trim(),moveInputs[3].value.trim()],
      level: parseInt(levelInput.value),
      evs: [parseInt(valueInputs[0].value),parseInt(valueInputs[1].value),parseInt(valueInputs[2].value),parseInt(valueInputs[3].value),parseInt(valueInputs[4].value),parseInt(valueInputs[5].value)],
      ivs: [parseInt(valueInputs[6].value),parseInt(valueInputs[7].value),parseInt(valueInputs[8].value),parseInt(valueInputs[9].value),parseInt(valueInputs[10].value),parseInt(valueInputs[11].value)],
    }
  }
}
