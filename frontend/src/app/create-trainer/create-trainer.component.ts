import { Component, ElementRef, Input, ViewChild } from '@angular/core';
import { CreateBattleComponent } from '../create-battle/create-battle.component';
import DataLists from 'src/DataLists';

@Component({
  selector: 'app-create-trainer',
  templateUrl: './create-trainer.component.html',
  styleUrls: ['./create-trainer.component.css']
})
export class CreateTrainerComponent {
  collapsed: boolean;
  @Input() dataLists: DataLists = new DataLists();
  constructor(){
    this.collapsed = false;
  }

  public toggleCollapsible() {
    this.collapsed = !this.collapsed;
  }
}
