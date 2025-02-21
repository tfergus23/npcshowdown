import { Component } from '@angular/core';

@Component({
  selector: 'app-user-trainers-modal',
  templateUrl: './user-trainers-modal.component.html',
  styleUrls: ['./user-trainers-modal.component.css']
})
export class UserTrainersModalComponent {

  hidden: boolean = true;

  close(e: MouseEvent){
    this.hidden = true;
  }
}
