import { Component } from '@angular/core';
import { UserService } from '../user.service';
import { AppComponent } from '../app.component';
import User from 'src/User';
import { HttpClient } from '@angular/common/http';
import { AuthenticationService } from '../authentication.service';
import {CookieService} from 'ngx-cookie-service';

@Component({
  selector: 'app-navbar',
  templateUrl: './navbar.component.html',
  styleUrls: ['./navbar.component.css']
})
export class NavbarComponent {
  constructor(public app: AppComponent, private authService: AuthenticationService, private userService: UserService, private cookieService: CookieService){}
  ngOnInit(): void {
  }
  login(){
    const username: string = (document.getElementById("user_input") as HTMLInputElement).value;
    const password: string = (document.getElementById("password_input") as HTMLInputElement).value;
    if (username == "" || password == ""){
      return;
    }
    const authResponse = this.authService.getToken(username,password);
    authResponse.subscribe((res) =>{
      if (res.success && res.token != undefined){
        console.log(res.token);
        const userResponse = this.userService.getUserData(res.token);
        userResponse.subscribe((res) => {
          if (res.success){
            this.app.loggedInUser = {
              name: res.name,
              id: res.id,
              token: res.token
            };
            this.cookieService.set("token", res.token);
          }
          else{
            //something went wrong
          }
        });
      }
      else{
        console.log(res.message);
      }
    });
  }
}
