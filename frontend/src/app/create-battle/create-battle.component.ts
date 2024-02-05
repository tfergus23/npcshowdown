import { Component } from '@angular/core';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';

@Component({
  selector: 'app-create-battle',
  templateUrl: './create-battle.component.html',
  styleUrls: ['./create-battle.component.css']
})
export class CreateBattleComponent {
  dataLists!: DataLists;
  constructor(private dataService: DataService){
  }

  ngOnInit() : void{
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
    });
  }
  
}
