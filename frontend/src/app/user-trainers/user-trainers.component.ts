import { Component } from '@angular/core';
import { AppComponent, MessageType } from '../app.component';
import Trainer from 'src/Trainer';
import { UserService } from '../user.service';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';

@Component({
  selector: 'app-user-trainers',
  templateUrl: './user-trainers.component.html',
  styleUrls: ['./user-trainers.component.css']
})
export class UserTrainersComponent {
  trainers: Array<Trainer> | undefined;
  dataLists!: DataLists;
  constructor(public app: AppComponent, private userService: UserService, private dataService: DataService){
    if (localStorage.getItem('user') != null)
    this.userService.getUserTrainers(localStorage.getItem('user') as string).subscribe((res) => {
      if (res.success){
        this.trainers = res.data;
      }
    }, 
    (error) => {
      this.app.logoutUser();
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });

    this.dataService.getAllData().subscribe((res) =>{
      this.dataLists = res.data;
    },
    (error) =>{

    });
  }

}
