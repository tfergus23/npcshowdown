import { Component, enableProdMode } from '@angular/core';
import User from '../User'
import { UserService } from './user.service';
import { AuthenticationService } from './authentication.service';
import { CookieService } from 'ngx-cookie-service';
import { trigger, state, style, animate, transition } from '@angular/animations';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
  animations: [
    trigger('fade', [
      state('visible', style({ opacity: 1, visibility: 'visible'})),
      state('hidden', style({ opacity: 0, visibility: 'hidden'})),
      transition('visible <=> hidden', animate('0.35s ease-in-out')),
    ])
  ]
})

export class AppComponent {
  loggedInUser: User | undefined = undefined;
  title = 'NPCShowdown';
  currentMessage: string = "qweqwe";
  hideMessage: boolean = true;
  currentMessageInterval: NodeJS.Timer | undefined = undefined;
  color: string = COLOR_INFO;

  constructor(private userService: UserService, private authService: AuthenticationService, private cookieService: CookieService){}
  ngOnInit(){
    if (localStorage.getItem('user') != null)
    this.setUserData(localStorage.getItem('user') as string);
  }

  setUserData(username: string){
    this.userService.getUserData(username).subscribe((res) =>{
      if (res.success){
        this.loggedInUser = res.data;
        localStorage.setItem('user', res.data.name);
      }
    },(error) => {
      this.logoutUser();
      this.showMessage(error.error.message, MessageType.ERROR);
    });
  }

  logoutUser(){
    this.loggedInUser = undefined;
    localStorage.removeItem('user');
  }

  showMessage(message: string, type: MessageType){
    switch (type){
      case MessageType.INFO:
        this.color = COLOR_INFO;
        break;
      case MessageType.ERROR:
        this.color = COLOR_ERROR;
        break;
    }
    this.currentMessage = message;
    this.hideMessage = false;
    this.startMessageHideCountdown();

  }

  stopHideCountdown(){
    clearInterval(this.currentMessageInterval);
  }

  startMessageHideCountdown(){
    clearInterval(this.currentMessageInterval);
    this.currentMessageInterval = setInterval(() => {
      this.hideMessage = true;
      clearInterval(this.currentMessageInterval);
    }, 3000);
  }
}

export enum MessageType{
  INFO, ERROR
}

const COLOR_INFO: string = "rgb(0, 255, 119)";
const COLOR_ERROR: string = "rgb(255, 0, 0)";