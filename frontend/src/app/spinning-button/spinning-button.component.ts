import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-spinning-button',
  templateUrl: './spinning-button.component.html',
  styleUrls: ['./spinning-button.component.css']
})
export class SpinningButtonComponent {
  @Input() click: Function = ()=>{};
  @Input() loadWhile: Function = ()=>{return false;};
  @Input() style: { [klass: string]: any; } = {};
  @Input() text: string = "";
  @Input() disabled: boolean = false;
}
