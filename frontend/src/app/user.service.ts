import { Injectable } from '@angular/core';
import User from '../User';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class UserService {

  constructor(private http: HttpClient) { }
  private userURL: string = "http://localhost:3000/api/user/BilboSwaggins";

  getUserData(id: number, authToken: string) : Observable<User>{
    const data = this.http.get<User>(this.userURL);
    return data;
  }
}
