import { Injectable } from '@angular/core';
import User from '../User';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';
import GetResponse from 'src/GetResponse';

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

  getUserData(username: string, authToken: string) : Observable<GetResponse>{
    const response = this.http.get<GetResponse>(`${AppSettings.API_URL}user/${username}`, {headers: {Authorization: authToken}});
    return response;
  }
}
