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
        const expirationDate = new Date();
        expirationDate.setFullYear(expirationDate.getFullYear() + 10);
        
        this.cookieService.set('token', `${username}:${res.token}`, expirationDate, '/');
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
    if (!this.cookieService.check('token')){
      this.dropdownDisplay = "none";
      this.app.loggedInUser = undefined;
      this.cookieService.delete('token');
      return;
    }
    const splitToken = this.cookieService.get('token').split(':');
    const username = splitToken[0];
    const token = splitToken[1];
    this.userService.logOut(username, token).subscribe((res) =>{
      if (res.success){
        this.dropdownDisplay = "none";
        this.app.loggedInUser = undefined;
        this.cookieService.delete('token');
      }
    }, (error) =>{
      if (error.status == 401){
        this.dropdownDisplay = "none";
        this.app.loggedInUser = undefined;
        this.cookieService.delete('token');
      }
      else{
        console.error(error);
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
