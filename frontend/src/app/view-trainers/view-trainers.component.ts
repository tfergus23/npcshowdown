import { Component } from '@angular/core';
import { DataService } from '../data.service';
import DataLists from 'src/DataLists';
import { AppComponent, MessageType } from '../app.component';

@Component({
  selector: 'app-view-trainers',
  templateUrl: './view-trainers.component.html',
  styleUrls: ['./view-trainers.component.css']
})
export class ViewTrainersComponent {
  dataLists!: DataLists;
  constructor(public app: AppComponent, private dataService: DataService){
    dataService.getAllData().subscribe((res) => {
      this.dataLists = res.data;
    },
    (error) => {
      this.app.showMessage("Sorry, looks like the service is down. Please try again later.", MessageType.ERROR);
    });
  }
}
