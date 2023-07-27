import { Injectable } from '@angular/core';
import User from '../User';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';

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

  getUserData(authToken: string) : Observable<UserResponse>{
    let name = authToken.split(":")[0];
    const response = this.http.get<UserResponse>(`${AppSettings.API_URL}user/${name}`, {headers: {Authorization: authToken}});
    return response;
  }

  logOut(authToken: string) : Observable<LogoutResponse>{
    let name = authToken.split(":")[0];
    const response = this.http.put<LogoutResponse>(`${AppSettings.API_URL}user/${name}/logout`, {headers: {Authorization: authToken}});
    return response;
  }
}
