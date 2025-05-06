import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { AppConfigService } from './app-config.service';
import { Observable } from 'rxjs';
import GetResponse from 'src/GetResponse';
import DeleteResponse from 'src/DeleteResponse';
import PostResponse from 'src/PostResponse';
import BattleRequest from 'src/BattleRequest';

@Injectable({
  providedIn: 'root'
})
export class AdminServiceService {

  constructor(private http: HttpClient, private config: AppConfigService) { }

  public getErrorBattles(username: string, page: number, count: number) : Observable<GetResponse>{
    const response = this.http.get<GetResponse>(`${this.config.getConfig().apiURL}user/${username}/errors?page=${page}&count=${count}`, {withCredentials: true});
    return response;
  }

  public getErrorBattleCount(username: string) : Observable<GetResponse>{
    const response = this.http.get<GetResponse>(`${this.config.getConfig().apiURL}user/${username}/errors/count`, {withCredentials: true});
    return response;
  }

  public acknowledgeErrorBattle(username: string, hash: string) : Observable<DeleteResponse>{
    const response = this.http.delete<DeleteResponse>(`${this.config.getConfig().apiURL}user/${username}/error/${hash}`, {withCredentials: true});
    return response;
  }

  public postErrorBattle(username: string, request: BattleRequest) : Observable<GetResponse>{
    const response = this.http.post<GetResponse>(`${this.config.getConfig().apiURL}user/${username}/error`, request, {withCredentials: true});
    return response;
  }
}
