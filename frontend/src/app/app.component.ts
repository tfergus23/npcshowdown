import { Component } from '@angular/core';
import User from '../User'
import { UserService } from './user.service';
import { AuthenticationService } from './authentication.service';
import { CookieService } from 'ngx-cookie-service';
import { jwtDecode } from 'jwt-decode';


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
    if (this.cookieService.check("token")){
      const token = this.cookieService.get("token");
      const decodedJWT = jwtDecode(token);
      const username = decodedJWT.sub!;
      this.userService.getUserData(username, token).subscribe((res) =>{
        if (res.success){
          this.loggedInUser = res.data;
        }
      });
  }
  }
}
