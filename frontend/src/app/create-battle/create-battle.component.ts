import { Component, ElementRef, QueryList, ViewChild, ViewChildren } from '@angular/core';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { BattleService } from '../battle.service';

@Component({
  selector: 'app-create-battle',
  templateUrl: './create-battle.component.html',
  styleUrls: ['./create-battle.component.css']
})
export class CreateBattleComponent {
  dataLists!: DataLists;
  @ViewChildren(CreateTrainerComponent) trainers!: QueryList<CreateTrainerComponent>;
  @ViewChild('seed') seed!: ElementRef<HTMLElement>;
  errors: Array<string> = [];
  logLines: Array<string> = [];
  constructor(private dataService: DataService, private battleService: BattleService){
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
    });
  }

  ngOnInit() : void{

  }

  submit(){
    let seedValue = this.seed.nativeElement.querySelector('input')!.value.trim();
    this.battleService.postBattleRequest({
      trainer1: this.trainers.get(0)!.getJSON(),
      trainer2: this.trainers.get(1)!.getJSON(),
      seed: seedValue == "" ? Math.round((Math.random() * 2147483647)).toString() : seedValue
    }).subscribe(response => {
      if (!response.success){
        this.errors = response.message.split("\n");
        this.errors = this.errors.filter((err) =>{
          return err != "";
        });
        this.logLines = [];
        return;
      }
      this.errors = [];
      this.logLines = response.message.split("\n");
      this.logLines = this.logLines.filter((line) =>{
        return line != "";
      });
    },
    (error: Error) => {
      this.errors = [];
      this.errors.push(error.message);
      this.errors = this.errors.filter((err) =>{
        return err != "";
      });
    });
  }
  
}
