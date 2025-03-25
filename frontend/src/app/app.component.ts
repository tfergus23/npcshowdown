import { Component, enableProdMode } from '@angular/core';
import User from '../User'
import { UserService } from './user.service';
import { AuthenticationService } from './authentication.service';
import { CookieService } from 'ngx-cookie-service';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  loggedInUser: User | undefined = undefined;
  title = 'NPCShowdown';

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
      console.error(error.error.message);
    });
  }

  logoutUser(){
    this.loggedInUser = undefined;
    localStorage.removeItem('user');
  }
}
