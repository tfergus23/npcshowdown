import { Injectable } from '@angular/core';
import User from '../User';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import GetResponse from 'src/GetResponse';
import { AppComponent } from './app.component';
import Trainer from 'src/Trainer';
import PostResponse from 'src/PostResponse';
import { AppConfigService } from './app-config.service';

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

interface PutResponse{
  success: boolean,
  message: string
}

@Injectable({
  providedIn: 'root'
})
export class UserService {

  constructor(private http: HttpClient, private config: AppConfigService) { }

  getUserData(username: string) : Observable<GetResponse>{
    username = encodeURIComponent(username);
    const response = this.http.get<GetResponse>(`${this.config.getConfig().apiURL}user/${username}`, {withCredentials: true});
    return response;
  }

  logOut(username: string) : Observable<LogoutResponse>{
    username = encodeURIComponent(username);
    const response = this.http.delete<LogoutResponse>(`${this.config.getConfig().apiURL}user/${username}/logout`, {withCredentials: true});
    return response;
  }

  addTrainerToUserProfile(username: string, trainer: Trainer) : Observable<PostResponse>{
    username = encodeURIComponent(username);
    const response = this.http.post<PostResponse>(`${this.config.getConfig().apiURL}user/${username}/trainer`, trainer, {withCredentials: true});
    return response;
  }

  addTournamentToUserProfile(username: string, tournamentID: number) : Observable<PostResponse>{
    username = encodeURIComponent(username);
    const response = this.http.post<PostResponse>(`${this.config.getConfig().apiURL}user/${username}/tournament/${tournamentID}`, {}, {withCredentials: true});
    return response;
  }

  getUserTrainers(username: string) : Observable<GetResponse>{
    username = encodeURIComponent(username);
    const response = this.http.get<GetResponse>(`${this.config.getConfig().apiURL}user/${username}/trainers`, {withCredentials: true});
    return response;
  }

  getUserTournaments(username: string) : Observable<GetResponse>{
    username = encodeURIComponent(username);
    const response = this.http.get<GetResponse>(`${this.config.getConfig().apiURL}user/${username}/tournaments`, {withCredentials: true});
    return response;
  }

  updateUserTrainer(username: string, trainer: Trainer) : Observable<PutResponse>{
    username = encodeURIComponent(username);
    const response = this.http.put<PutResponse>(`${this.config.getConfig().apiURL}user/${username}/trainer/${trainer.id!}`, trainer, {withCredentials: true});
    return response;
  }

  deleteUserTrainer(username: string, trainer: number) : Observable<PutResponse>{
    username = encodeURIComponent(username);
    const response = this.http.delete<PutResponse>(`${this.config.getConfig().apiURL}user/${username}/trainer/${trainer}`, {withCredentials: true});
    return response;
  }

  deleteTournamentFromUserProfile(username: string, tournamentID: number) : Observable<PutResponse>{
    username = encodeURIComponent(username);
    const response = this.http.delete<PutResponse>(`${this.config.getConfig().apiURL}user/${username}/tournament/${tournamentID}`, {withCredentials: true});
    return response;
  }

  updateUserEmail(username: string, newEmail: string) : Observable<PutResponse>{
    username = encodeURIComponent(username);
    const response = this.http.put<PutResponse>(`${this.config.getConfig().apiURL}user/${username}/email`, {newEmail: newEmail}, {withCredentials: true});
    return response;
  }

  updateUserPassword(username: string, currentPassword: string, newPassword: string) : Observable<PutResponse> {
    username = encodeURIComponent(username);
    const response = this.http.put<PutResponse>(`${this.config.getConfig().apiURL}user/${username}/password`, {newPassword: newPassword, currentPassword: currentPassword}, {withCredentials: true});
    return response;
  }

  createAccount(username: string, password: string) : Observable<PostResponse> {
    username = encodeURIComponent(username);
    const response = this.http.post<PostResponse>(`${this.config.getConfig().apiURL}user`, {username: username, password: password}, {withCredentials: true});
    return response;
  }

  deleteUser(username: string) : Observable<PutResponse> {
    username = encodeURIComponent(username);
    const response = this.http.delete<PutResponse>(`${this.config.getConfig().apiURL}user/${username}`, {withCredentials: true});
    return response;
  }
}
