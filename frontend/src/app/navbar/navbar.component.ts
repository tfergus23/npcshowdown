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
  dropdownDisplay: string = "none";

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
        this.cookieService.set('token', `${username}:${res.token}`);
        this.app.setUserData();
      }
      else{
        console.error(res.message);
      }
    }, (error) => {
      console.error(error.error.message);
    });
  }
  logout(){
    const splitToken = this.cookieService.get('token').split(':');
    const username = splitToken[0];
    const token = splitToken[1];
    this.userService.logOut(username, token).subscribe((res) =>{
      if (res.success){
        this.dropdownDisplay = "none";
        this.app.loggedInUser = undefined;
        this.cookieService.delete('token');
      }
    });

  }

  showDropDown(){
    if (this.dropdownDisplay == "block"){
      this.dropdownDisplay = "none";
    }
    else{
      this.dropdownDisplay = "block";
    }
  }

  hideDropDown(){
    //this.dropdownDisplay = "none";
  }
}
