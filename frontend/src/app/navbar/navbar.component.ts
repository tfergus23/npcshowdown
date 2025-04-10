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
  showUserDropdown: boolean = false;

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
      this.showUserDropdown = false;
      this.app.logoutUser();
      this.app.showMessage("Logout successful!", MessageType.INFO);
      return;
    }
    this.userService.logOut(localStorage.getItem('user') as string).subscribe((res) =>{
      if (res.success){
        this.showUserDropdown = false;
        this.app.logoutUser();
        this.app.showMessage("Logout successful!", MessageType.INFO);
      }
    }, (error) =>{
      if (error.status == 401){
        this.showUserDropdown = false;
        this.app.logoutUser();
        this.app.showMessage("Session expired", MessageType.ERROR);
      }
      else{
        this.showUserDropdown = false;
        this.app.logoutUser();
        this.app.showMessage("Session expired", MessageType.ERROR);
      }
    });

  }

  toggleUserDropDown(){
    this.showUserDropdown = !this.showUserDropdown;
  }

  hideDropDown(){
    if (this.showUserDropdown)
    this.showUserDropdown = false;
  }

  showMenuDropDown(){

  }

  hideMenuDropDown(){

  }
}
