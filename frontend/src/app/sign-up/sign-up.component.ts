import { Component } from '@angular/core';
import { UserService } from '../user.service';
import { AppComponent, MessageType } from '../app.component';
import { Router } from '@angular/router';
import { AuthenticationService } from '../authentication.service';

@Component({
  selector: 'app-sign-up',
  templateUrl: './sign-up.component.html',
  styleUrls: ['./sign-up.component.css']
})
export class SignUpComponent {
  fields: SignUpFields = {
    username: "",
    password: "",
    confirmPassword: ""
  };

  constructor(public app: AppComponent, private userService: UserService, private route: Router, private authService: AuthenticationService){}

  ngOnInit(){
    if (localStorage.getItem('user') != null){
      this.route.navigateByUrl('/');
    }
  }

  submitCreateUser(){
    this.fields.username = this.fields.username.trim();
    if (this.fields.username == ""){
      this.app.showMessage("Please enter a valid username.", MessageType.ERROR);
      return;
    }
    if (this.fields.password == ""){
      this.app.showMessage("Please enter a password.", MessageType.ERROR);
      return;
    }
    if (this.fields.confirmPassword == ""){
      this.app.showMessage("Please confirm your password.", MessageType.ERROR);
      return;
    }
    if (this.fields.password !== this.fields.confirmPassword){
      this.app.showMessage("Passwords do not match!", MessageType.ERROR);
      return;
    }
    this.userService.createAccount(this.fields.username, this.fields.password).subscribe((res) => {
      if (res.success){
        this.authService.getToken(this.fields.username, this.fields.password).subscribe((res) =>{
          if (res.success){
            this.app.setUserData(this.fields.username);
            this.app.showMessage(`Account created! Welcome, ${this.fields.username}!`, MessageType.INFO);
            this.route.navigateByUrl('/');
          }
          else{
            this.app.showMessage(res.message, MessageType.ERROR);
          }
        },
        (error) =>{
          this.app.showMessage(error.error.message, MessageType.ERROR);
        });
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) => {
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
  }

}

interface SignUpFields{
  username: string,
  password: string,
  confirmPassword: string
}