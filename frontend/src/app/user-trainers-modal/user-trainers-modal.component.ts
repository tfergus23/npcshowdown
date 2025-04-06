import { Component, Input } from '@angular/core';
import { AppComponent, MessageType } from '../app.component';
import Trainer from 'src/Trainer';
import { UserService } from '../user.service';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';

@Component({
  selector: 'app-user-trainers-modal',
  templateUrl: './user-trainers-modal.component.html',
  styleUrls: ['./user-trainers-modal.component.css']
})
export class UserTrainersModalComponent {

  @Input() parent!: CreateTrainerComponent;
  trainers: Array<Trainer> | undefined;
  selectedIndex: number = -1;
  hidden: boolean = true;

  constructor(public app: AppComponent, private userSerivce: UserService){
  }

  show(){
    this.userSerivce.getUserTrainers(localStorage.getItem('user') as string).subscribe((res) => {
      if (res.success){
        this.trainers = res.data;
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
        this.close(new MouseEvent(''));
      }
    },
    (error) =>{
      if (error.status == 401){
        this.app.logoutUser();
      }
      this.app.showMessage(error.error.message, MessageType.ERROR);
      this.close(new MouseEvent(''));
    });
    this.hidden = false;
  }
  
  close(e: MouseEvent){
    this.hidden = true;
    this.selectedIndex = -1;
  }

  selectTrainer(index: number){
    this.selectedIndex = index;
  }

  importSelectedTrainer(){
    if (this.selectedIndex >= 0)
    this.parent.setFromJson(this.trainers![this.selectedIndex]);
    this.close(new MouseEvent(''));
  }
}
