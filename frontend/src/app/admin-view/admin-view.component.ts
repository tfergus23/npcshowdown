import { Component, ViewChild } from '@angular/core';
import { AppComponent, MessageType } from '../app.component';
import { BattleLogViewComponent } from '../battle-log-view/battle-log-view.component';
import { BattleService } from '../battle.service';
import { AdminServiceService } from '../admin-service.service';

interface ErrorBattle{
  battle: string,
  dateRan: string,
  hash: string
}

@Component({
  selector: 'app-admin-view',
  templateUrl: './admin-view.component.html',
  styleUrls: ['./admin-view.component.css']
})
export class AdminViewComponent {
  numErrors: number = 0;
  errorBattles: Array<ErrorBattle> = [];
  page: number = 0;
  totalBattles: number = 0;
  BATTLES_PER_PAGE = 9;
  @ViewChild("logView") battleLogView!: BattleLogViewComponent;
  constructor(public app: AppComponent, private battleService: BattleService, private adminService: AdminServiceService){
    this.getErrorBattles();
  }

  getErrorBattles(){
    this.adminService.getErrorBattles(localStorage.getItem('user') as string, this.page, this.BATTLES_PER_PAGE).subscribe((res) => {
      if (res.success){
        this.errorBattles = res.data;
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) => {
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
    this.adminService.getErrorBattleCount(localStorage.getItem('user') as string).subscribe((res) =>{
      this.totalBattles = res.data;
    });
  }

  viewBattle(index: number){
    let battle: ErrorBattle = this.errorBattles[index];
    this.adminService.postErrorBattle(this.app.loggedInUser!.name,JSON.parse(battle.battle)).subscribe((res) =>{
      if (res.success){
        this.battleLogView.log = res.data;
        this.battleLogView.hidden = false;
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) =>{
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
  }

  downloadBattle(index: number){
    let battle: ErrorBattle = this.errorBattles[index];
    const newBlob = new Blob([battle.battle], {
      type: 'application/json'
    });
    const data = window.URL.createObjectURL(newBlob);
    const link = document.createElement("a");
    link.href = data;
    link.download = `error_battle_${battle.hash}.json`; 
    link.click();
    link.remove();
  }

  acknowledgeBattle(index: number){
    let battle: ErrorBattle = this.errorBattles[index];
    this.adminService.acknowledgeErrorBattle(localStorage.getItem('user') as string, battle.hash).subscribe((res) => {
      if (res.success){
        this.getErrorBattles();
        this.app.showMessage("Error battle acknowledged.", MessageType.INFO);
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) => {
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
  }

  battlesOnPage(){
    return Math.min(this.page * this.BATTLES_PER_PAGE + this.BATTLES_PER_PAGE, this.totalBattles);
  }

  changePage(amount: number){
    if (amount > 0 && this.battlesOnPage() == this.totalBattles){
      return;
    }
    if (amount < 0 && this.page == 0){
      return;
    }
    this.page += amount;
    this.getErrorBattles();
  }
}
