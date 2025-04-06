import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-battle-log-view',
  templateUrl: './battle-log-view.component.html',
  styleUrls: ['./battle-log-view.component.css']
})
export class BattleLogViewComponent {
  @Input() log?: string = "Test log!\nThis should be on a new line!";
  hidden: boolean = true;
  onHide?: Function;

  hide(){
    this.hidden = true;
    if (this.onHide){
      this.onHide();
    }
  }

  download(){
    const newBlob = new Blob([this.log!], {
      type: 'application/json'
    });
    const data = window.URL.createObjectURL(newBlob);
    const link = document.createElement("a");
    link.href = data;
    link.download = `battle_log.txt`; 
    link.click();
    link.remove();
  }
}
