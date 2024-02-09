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
  pokeComponents: Array<CreatePokemonComponent> = new Array<CreatePokemonComponent>();
  constructor(){
    this.collapsed = false;
  }

  public toggleCollapsible() {
    this.collapsed = !this.collapsed;
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
    const name: string = "Joey";
    const trainerClass: string = "Youngster";
    const level: string = "Wild";
    let pokemon = new Array<Pokemon>();
    for(let i = 0; i < this.pokeComponents.length; i++){
      pokemon.push(this.pokeComponents[i].getJSON());
    }
    return {
      name: name,
      trainerClass: trainerClass,
      trainerLevel: level,
      team: pokemon
    };
  }

}
