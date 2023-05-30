import { Injectable } from '@angular/core';
import User from '../User';

@Injectable({
  providedIn: 'root'
})
export class UserService {

  constructor() { }

  getUserData(id: number, authToken: string) : User | undefined{
    if (authToken == "token")
    return {
      id: 1,
      name: "iiii"
    };
    return undefined;
  }
}
