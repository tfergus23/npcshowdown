import { Component, ElementRef, Input, ViewChild } from '@angular/core';
import DataLists from 'src/DataLists';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { bootstrapApplication } from '@angular/platform-browser';
import Pokemon from 'src/Pokemon';

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

  getJSON() : Pokemon{
    const speciesInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-species input') as HTMLInputElement;
    const nicknameInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-nickname input') as HTMLInputElement;
    const abilitySelect: HTMLSelectElement = this.fields.nativeElement.querySelector('.pokemon-ability select') as HTMLSelectElement;
    const itemSelect: HTMLSelectElement = this.fields.nativeElement.querySelector('.pokemon-item select') as HTMLSelectElement;
    const natureSelect: HTMLSelectElement = this.fields.nativeElement.querySelector('.pokemon-nature select') as HTMLSelectElement;
    const genderSelect: HTMLSelectElement = this.fields.nativeElement.querySelector('.pokemon-gender select') as HTMLSelectElement;
    const moveSelects: NodeListOf<HTMLSelectElement> = this.fields.nativeElement.querySelectorAll('.pokemon-move select');
    const levelInput: HTMLInputElement = this.fields.nativeElement.querySelector('.pokemon-level input') as HTMLInputElement;
    const valueInputs: NodeListOf<HTMLInputElement> = this.fields.nativeElement.querySelectorAll('.evs-ivs input');


    return {
      species: speciesInput.value,
      nickname: nicknameInput.value,
      abilityName: abilitySelect.value,
      itemName: itemSelect.value,
      nature: natureSelect.value,
      gender: genderSelect.value,
      moves: [moveSelects[0].value,moveSelects[1].value,moveSelects[2].value,moveSelects[3].value],
      level: parseInt(levelInput.value),
      evs: [parseInt(valueInputs[0].value),parseInt(valueInputs[1].value),parseInt(valueInputs[2].value),parseInt(valueInputs[3].value),parseInt(valueInputs[4].value),parseInt(valueInputs[5].value)],
      ivs: [parseInt(valueInputs[6].value),parseInt(valueInputs[7].value),parseInt(valueInputs[8].value),parseInt(valueInputs[9].value),parseInt(valueInputs[10].value),parseInt(valueInputs[11].value)],
    }
  }
}
