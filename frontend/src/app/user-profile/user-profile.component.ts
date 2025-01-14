import { Component } from '@angular/core';
import { AppComponent } from '../app.component';
import { UserService } from '../user.service';
import { CookieService } from 'ngx-cookie-service';
import { resetFakeAsyncZone } from '@angular/core/testing';

@Component({
  selector: 'app-user-profile',
  templateUrl: './user-profile.component.html',
  styleUrls: ['./user-profile.component.css']
})
export class UserProfileComponent {
  constructor(public app: AppComponent, private userService: UserService, private cookieService: CookieService){}

  ngOnInit(){
    this.app.setUserData();
  }
}
