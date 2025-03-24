import { Component } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { AppComponent } from '../app.component';

@Component({
  selector: 'app-user-view',
  templateUrl: './user-view.component.html',
  styleUrls: ['./user-view.component.css']
})
export class UserViewComponent {

  selectedStyle: string = "text-decoration: underline; background-color: var(--light-background-color);";

  constructor(public activatedRoute: ActivatedRoute, public app: AppComponent){
  }
}
