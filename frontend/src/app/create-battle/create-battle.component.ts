import { Component, ElementRef, QueryList, ViewChild, ViewChildren } from '@angular/core';
import DataLists from 'src/DataLists';
import { DataService } from '../data.service';
import { CreateTrainerComponent } from '../create-trainer/create-trainer.component';
import { BattleService } from '../battle.service';
import { BattleLogViewComponent } from '../battle-log-view/battle-log-view.component';

const SERVICE_DOWN_RESPONSE = "Sorry, it looks like the service is down. Please try again some other time.";

@Component({
  selector: 'app-create-battle',
  templateUrl: './create-battle.component.html',
  styleUrls: ['./create-battle.component.css']
})
export class CreateBattleComponent {
  dataLists?: DataLists;
  @ViewChildren(CreateTrainerComponent) trainers!: QueryList<CreateTrainerComponent>;
  @ViewChild('seed') seed!: ElementRef<HTMLElement>;
  errors: Array<string> = [];
  logLines: Array<string> = [];
  showingBattle: boolean = false;
  submittingBattle: boolean = false;
  whileSubmittingBattle = () => {return this.submittingBattle;}
  @ViewChild('logView') logView!: BattleLogViewComponent
  constructor(private dataService: DataService, private battleService: BattleService){
    this.dataService.getAllData().subscribe((response) => {
      if (!response.success) return;
      this.dataLists = response!.data;
    },
    (error) => {
      this.errors.push(SERVICE_DOWN_RESPONSE)
    });
  }
  pointerEvents?: string;

  ngAfterViewInit(){
    setTimeout(() => {this.loadEntries()});
  }

  submit = () => {
    this.submittingBattle = true;
    let seedValue = this.seed.nativeElement.querySelector('input')!.value.trim();
    this.battleService.postBattleRequest({
      trainer1: this.trainers.get(0)!.getJSON(),
      trainer2: this.trainers.get(1)!.getJSON(),
      seed: seedValue == "" ? Math.round((Math.random() * 2147483647)).toString() : seedValue
    }).subscribe(
      (response) => {
        this.submittingBattle = false;
        this.errors = [];
        this.logView.log = response.data;
        this.logView.hidden = false;
        
      },
      (error) => {
        this.submittingBattle = false;
        this.showErrorResponse(error);
      });
  }

  onBattleClose(){
  }

  showErrorResponse(error: any){
    this.errors = [];
    this.logLines = [];
    if (error.error.message){
      const response = error.error;
      this.errors = response.message.split("\n");
      this.errors = this.errors.filter((err) =>{
        return err != "";
      });
    }
    else{
      this.errors.push(SERVICE_DOWN_RESPONSE);
    }
  }

  saveEntries = () => {
    const entries = {
      trainer1: this.trainers.get(0)!.getJSON(),
      trainer2: this.trainers.get(1)!.getJSON(),
      seed: this.seed.nativeElement.querySelector('input')!.value.trim()
    };
    localStorage.setItem('create-battle', JSON.stringify(entries));
  }

  loadEntries(){
    const entriesString = localStorage.getItem('create-battle');
    if (entriesString != null){
      const entries = JSON.parse(entriesString);
      this.trainers.get(0)!.setFromJson(entries.trainer1);
      this.trainers.get(1)!.setFromJson(entries.trainer2);
      this.seed.nativeElement.querySelector('input')!.value = entries.seed;
    }
  }
  
}
