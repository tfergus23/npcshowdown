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
      if (res.success){
        this.app.setUserData(username);
      }
      else{
        console.error(res.message);
      }
    }, (error) => {
      console.error(error.error.message);
    });
  }
  logout(){
    if (localStorage.getItem('user') == null){
      this.dropdownDisplay = "none";
      this.app.loggedInUser = undefined;
      localStorage.removeItem('user');
      return;
    }
    this.userService.logOut(localStorage.getItem('user') as string).subscribe((res) =>{
      if (res.success){
        this.dropdownDisplay = "none";
        this.app.loggedInUser = undefined;
        localStorage.removeItem('user');
      }
    }, (error) =>{
      if (error.status == 401){
        this.dropdownDisplay = "none";
        this.app.loggedInUser = undefined;
        localStorage.removeItem('user');
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
