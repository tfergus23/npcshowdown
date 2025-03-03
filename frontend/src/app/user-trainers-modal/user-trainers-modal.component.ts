import { Component } from '@angular/core';
import { AppComponent } from '../app.component';
import Trainer from 'src/Trainer';

@Component({
  selector: 'app-user-trainers-modal',
  templateUrl: './user-trainers-modal.component.html',
  styleUrls: ['./user-trainers-modal.component.css']
})
export class UserTrainersModalComponent {

  trainers: Array<Trainer> | undefined;

  constructor(public app: AppComponent){
    
  }


   
  hidden: boolean = true;

  close(e: MouseEvent){
    this.hidden = true;
  }
}
