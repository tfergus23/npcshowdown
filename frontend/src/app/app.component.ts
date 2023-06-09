import { Component } from '@angular/core';
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
    if (this.cookieService.check("token"))
    this.userService.getUserData(this.cookieService.get("token")).subscribe((res) =>{
      if (res.success){
        this.loggedInUser = {
          name:res.name,
          id:res.id,
          token:res.token
        }
      }
    });
    
  }
}
