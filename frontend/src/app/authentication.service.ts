import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { AppConfigService } from './app-config.service';

interface AuthResponse{
  success: boolean;
  token: string | undefined;
  message: string;
}

@Injectable({
  providedIn: 'root'
})

export class AuthenticationService {

  constructor(private http: HttpClient, private config: AppConfigService) { }

  getToken(username: string, password: string): Observable<AuthResponse>{
    const response = this.http.post<AuthResponse>(this.config.getConfig().apiURL + 'auth', {username: username, password: password}, {headers:{'Content-Type': 'application/json'}, withCredentials: true});
    return response;
  }
}
