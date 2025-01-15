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
    this.setUserData();
  }

  setUserData(){
    const hasToken = this.cookieService.check("token");
    console.log(hasToken);
    console.log(document.cookie);
    if (hasToken){
      console.log("app loaded");
      const splitToken = this.cookieService.get("token").split(":");
      const username = splitToken[0];
      const token = splitToken[1];
      
      this.userService.getUserData(username, token).subscribe((res) =>{
        if (res.success){
          this.loggedInUser = res.data;
        }
      },(error) => {
        this.loggedInUser = undefined;
        console.error(error.error.message);
      });
    }
  }
}
