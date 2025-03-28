import { Injectable } from '@angular/core';
import User from '../User';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';
import GetResponse from 'src/GetResponse';
import { AppComponent } from './app.component';
import Trainer from 'src/Trainer';
import PostResponse from 'src/PostResponse';

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

  constructor(private http: HttpClient) { }

  getUserData(username: string) : Observable<GetResponse>{
    const response = this.http.get<GetResponse>(`${AppSettings.API_URL}user/${username}`, {withCredentials: true});
    return response;
  }

  logOut(username: string) : Observable<LogoutResponse>{
    const response = this.http.delete<LogoutResponse>(`${AppSettings.API_URL}user/${username}/logout`, {withCredentials: true});
    return response;
  }

  addTrainerToUserProfile(username: string, trainer: Trainer) : Observable<PostResponse>{
    const response = this.http.post<PostResponse>(`${AppSettings.API_URL}user/${username}/trainer`, trainer, {withCredentials: true});
    return response;
  }

  addTournamentToUserProfile(username: string, tournamentID: number) : Observable<PostResponse>{
    const response = this.http.post<PostResponse>(`${AppSettings.API_URL}user/${username}/tournament/${tournamentID}`, {}, {withCredentials: true});
    return response;
  }

  getUserTrainers(username: string) : Observable<GetResponse>{
    const response = this.http.get<GetResponse>(`${AppSettings.API_URL}user/${username}/trainers`, {withCredentials: true});
    return response;
  }

  getUserTournaments(username: string) : Observable<GetResponse>{
    const response = this.http.get<GetResponse>(`${AppSettings.API_URL}user/${username}/tournaments`, {withCredentials: true});
    return response;
  }

  updateUserTrainer(username: string, trainer: Trainer) : Observable<PutResponse>{
    const response = this.http.put<PutResponse>(`${AppSettings.API_URL}user/${username}/trainer/${trainer.id!}`, trainer, {withCredentials: true});
    return response;
  }

  deleteUserTrainer(username: string, trainer: number) : Observable<PutResponse>{
    const response = this.http.delete<PutResponse>(`${AppSettings.API_URL}user/${username}/trainer/${trainer}`, {withCredentials: true});
    return response;
  }

  deleteTournamentFromUserProfile(username: string, tournamentID: number) : Observable<PutResponse>{
    const response = this.http.delete<PutResponse>(`${AppSettings.API_URL}user/${username}/tournament/${tournamentID}`, {withCredentials: true});
    return response;
  }

  updateUserEmail(username: string, newEmail: string) : Observable<PutResponse>{
    const response = this.http.put<PutResponse>(`${AppSettings.API_URL}user/${username}/email`, {newEmail: newEmail}, {withCredentials: true});
    return response;
  }

  updateUserPassword(username: string, currentPassword: string, newPassword: string) : Observable<PutResponse> {
    const response = this.http.put<PutResponse>(`${AppSettings.API_URL}user/${username}/password`, {newPassword: newPassword, currentPassword: currentPassword}, {withCredentials: true});
    return response;
  }
}
