import { Component, Input } from '@angular/core';
import { AppComponent, MessageType } from '../app.component';
import Trainer from 'src/Trainer';
import { UserService } from '../user.service';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { CreateTournamentComponent } from '../create-tournament/create-tournament.component';

@Component({
  selector: 'app-user-trainers-modal',
  templateUrl: './user-trainers-modal.component.html',
  styleUrls: ['./user-trainers-modal.component.css']
})
export class UserTrainersModalComponent {

  @Input() parent!: CreateTrainerComponent | CreateTournamentComponent;
  @Input() selectMultiple: boolean = false;
  trainers: Array<Trainer> | undefined;
  selectedIndecies: Array<number> = [];
  hidden: boolean = true;
  fetchingTrainers: boolean = false;

  constructor(public app: AppComponent, private userSerivce: UserService){
  }

  show(){
    this.fetchingTrainers = true;
    this.userSerivce.getUserTrainers(localStorage.getItem('user') as string).subscribe((res) => {
      this.fetchingTrainers = false;
      if (res.success){
        this.trainers = res.data;
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
        this.close(new MouseEvent(''));
      }
    },
    (error) =>{
      this.fetchingTrainers = false;
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
    this.selectedIndecies = [];
  }

  selectTrainer(index: number){
    if (this.selectMultiple){
      if (this.selectedIndecies.includes(index)){
        this.selectedIndecies.splice(this.selectedIndecies.indexOf(index), 1);
      }
      else{
        this.selectedIndecies.push(index);
      }
    }
    else{
      if (this.selectedIndecies.includes(index)){
        return;
      }
      else{
        this.selectedIndecies.pop();
        this.selectedIndecies.push(index);
      }
    }
  }

  importSelectedTrainers(){
    if (this.selectedIndecies.length > 0){
      if (this.parent instanceof CreateTrainerComponent){
        this.parent.setFromJson(this.trainers![this.selectedIndecies[0]]);
        this.parent.callOnEdit();
      }
      else if (this.parent instanceof CreateTournamentComponent){
        for (let index of this.selectedIndecies){
          this.parent.trainers.push(this.trainers![index]);
        }
        this.parent.saveEntries();
      }
    }
    this.close(new MouseEvent(''));
  }

  onDoubleClick(){
    if (!this.selectMultiple){
      this.importSelectedTrainers();
    }
  }
}
