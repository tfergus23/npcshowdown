import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';

interface AuthResponse{
  success: boolean;
  token: string | undefined;
  message: string;
}

@Injectable({
  providedIn: 'root'
})

export class AuthenticationService {

  constructor(private http: HttpClient) { }

  getToken(username: string, password: string): Observable<AuthResponse>{
    const response = this.http.post<AuthResponse>(AppSettings.API_URL + 'auth', {username: username, password: password}, {headers:{'Content-Type': 'application/json'}});
    return response;
  }
}
