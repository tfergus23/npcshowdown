import { Component } from '@angular/core';
import { UserService } from '../user.service';
import { AppComponent, MessageType } from '../app.component';
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

  displayMenuDropdown: boolean = true;

  constructor(public app: AppComponent, private authService: AuthenticationService, private userService: UserService, private cookieService: CookieService){}
  ngOnInit(): void {
  }
  login(){
    const username: string = (document.getElementById("user_input") as HTMLInputElement).value;
    const password: string = (document.getElementById("password_input") as HTMLInputElement).value;
    if (username == "" || password == ""){
      this.app.showMessage("Please enter your username and password.", MessageType.ERROR);
      return;
    }
    const authResponse = this.authService.getToken(username,password);
    authResponse.subscribe((res) =>{
      if (res.success){
        this.app.setUserData(username);
        this.app.showMessage("Login successful!", MessageType.INFO);
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    }, (error) => {
      this.app.showMessage(error.error.message, MessageType.ERROR)
    });
  }
  logout(){
    if (localStorage.getItem('user') == null){
      this.dropdownDisplay = "none";
      this.app.logoutUser();
      this.app.showMessage("Logout successful!", MessageType.INFO);
      return;
    }
    this.userService.logOut(localStorage.getItem('user') as string).subscribe((res) =>{
      if (res.success){
        this.dropdownDisplay = "none";
        this.app.logoutUser();
        this.app.showMessage("Logout successful!", MessageType.INFO);
      }
    }, (error) =>{
      if (error.status == 401){
        this.dropdownDisplay = "none";
        this.app.logoutUser();
        this.app.showMessage("Session expired", MessageType.ERROR);
      }
      else{
        this.app.showMessage("Session expired", MessageType.ERROR);
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

  showMenuDropDown(){

  }

  hideMenuDropDown(){

  }
}
