import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-battle-log-view',
  templateUrl: './battle-log-view.component.html',
  styleUrls: ['./battle-log-view.component.css']
})
export class BattleLogViewComponent {
  @Input() battleID?: number;
  @Input() log?: string = "Test log!\nThis should be on a new line!";
  hidden: boolean = true;
  onHide?: Function;

  hide(){
    this.hidden = true;
    if (this.onHide){
      console.log("calling onhide");
      this.onHide();
    }
  }
}
