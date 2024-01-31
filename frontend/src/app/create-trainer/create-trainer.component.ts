import { Component, ElementRef, ViewChild } from '@angular/core';

@Component({
  selector: 'app-create-trainer',
  templateUrl: './create-trainer.component.html',
  styleUrls: ['./create-trainer.component.css']
})
export class CreateTrainerComponent {
  collapsed: boolean;
  constructor(){
    this.collapsed = false;
  }

  public toggleCollapsible() {
    this.collapsed = !this.collapsed;
  }
}
