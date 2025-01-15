import { Injectable } from '@angular/core';
import User from '../User';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';
import GetResponse from 'src/GetResponse';
import { AppComponent } from './app.component';

interface UserResponse{
  name: string,
  id: number,
  success: boolean,
  token: string
}

interface LogoutResponse{
  success: boolean,
  message: string
}

@Injectable({
  providedIn: 'root'
})
export class UserService {

  constructor(private http: HttpClient) { }

  getUserData(username: string) : Observable<GetResponse>{
    const response = this.http.get<GetResponse>(`${AppSettings.API_URL}user/${username}`, {withCredentials: true});
    return response;
  }

  logOut(username: string) : Observable<LogoutResponse>{
    const response = this.http.delete<LogoutResponse>(`${AppSettings.API_URL}user/${username}/logout`, {withCredentials: true});
    return response;
  }
}
