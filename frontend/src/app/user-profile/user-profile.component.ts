import { Component } from '@angular/core';
import { AppComponent, MessageType } from '../app.component';
import { UserService } from '../user.service';
import { CookieService } from 'ngx-cookie-service';
import { resetFakeAsyncZone } from '@angular/core/testing';
import { Router } from '@angular/router';

@Component({
  selector: 'app-user-profile',
  templateUrl: './user-profile.component.html',
  styleUrls: ['./user-profile.component.css']
})
export class UserProfileComponent {
  hideChangePasswordModal: boolean = true;
  deleteStage: number = 0;
  disableDeleteButton: boolean = true;

  constructor(public app: AppComponent, private userService: UserService, private cookieService: CookieService, private router: Router){}

  ngOnInit(){
    if (localStorage.getItem('user') != null)
    this.app.setUserData(localStorage.getItem('user') as string);
  }

  updateEmail(){
    const emailInput = document.getElementById('email-input') as HTMLInputElement;
    this.userService.updateUserEmail(this.app.loggedInUser!.name, emailInput.value).subscribe((res) =>{
      if (res.success){
        this.app.showMessage("Email updated!", MessageType.INFO);
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) => {
      this.app.showMessage(error.error.message, MessageType.ERROR);
    });
  }

  openPasswordModal(){
    this.hideChangePasswordModal = false;
  }

  closePasswordModal(e: MouseEvent){
    this.hideChangePasswordModal = true;
    const currentPassword: HTMLInputElement = (document.getElementById('current-password') as HTMLInputElement);
    const newPassword: HTMLInputElement = (document.getElementById('new-password') as HTMLInputElement);
    const repeatPassword: HTMLInputElement = (document.getElementById('repeat-password') as HTMLInputElement);

    currentPassword.value = "";
    newPassword.value = "";
    repeatPassword.value = "";
  }

  updatePassword(){
    const currentPassword: string = (document.getElementById('current-password') as HTMLInputElement).value;
    const newPassword: string = (document.getElementById('new-password') as HTMLInputElement).value;
    const repeatPassword: string = (document.getElementById('repeat-password') as HTMLInputElement).value;

    if (newPassword !== repeatPassword){
      this.app.showMessage("Passwords do not match!", MessageType.ERROR);
      return;
    }

    this.userService.updateUserPassword(this.app.loggedInUser!.name, currentPassword, newPassword).subscribe((res) =>{
      if (res.success){
        this.closePasswordModal(new MouseEvent(''));
        this.app.showMessage("Password updated!", MessageType.INFO);
        this.app.setUserData(this.app.loggedInUser!.name);
      }
      else{
        this.app.showMessage(res.message, MessageType.ERROR);
      }
    },
    (error) => {
      if (error.status == 401){
        this.closePasswordModal(new MouseEvent(''));
        this.app.logoutUser();
        this.app.showMessage("Session expired", MessageType.ERROR);
      }
      else{
        this.app.showMessage(error.error.message, MessageType.ERROR);
      }
    });
  }

  openDeleteModal(){
    this.deleteStage++;
  }

  closeDeleteModal(){
    this.deleteStage = 0;
  }

  openConfirm(){
    this.deleteStage++;
    this.disableDeleteButton = true;
    setTimeout(() => {
      this.disableDeleteButton = false;
    }, 1500)
  }

  deleteAccount(){
    this.deleteStage = 0;
    this.userService.deleteUser(this.app.loggedInUser!.name).subscribe((res) => {
      if (res.success){
        this.app.logoutUser();
        this.router.navigateByUrl('/');
        this.app.showMessage("Your account has been successfully deleted.", MessageType.INFO);
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
