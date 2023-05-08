import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class AuthenticationService {

  constructor() { }

  getToken(username: string, password: string): string{
    return "token";
  }
}
