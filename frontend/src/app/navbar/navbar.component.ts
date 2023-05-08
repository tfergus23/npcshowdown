import { Component } from '@angular/core';
import { UserService } from '../user.service';
import User from 'src/User';

@Component({
  selector: 'app-navbar',
  templateUrl: './navbar.component.html',
  styleUrls: ['./navbar.component.css']
})
export class NavbarComponent {
  loggedInUser: User | undefined = undefined;
  constructor(private userService: UserService){}
  ngOnInit(): void {
    this.loggedInUser = this.userService.getUserData(1, "token");
  }
}
