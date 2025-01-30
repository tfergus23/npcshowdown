import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';
import BattleRequest from 'src/BattleRequest';
import GetResponse from 'src/GetResponse';
import PostResponse from 'src/PostResponse';
import TournamentRequest from 'src/TournamentRequest';
import { TournamentResultSet } from 'src/TournamentResultSet';

@Injectable({
  providedIn: 'root'
})
export class BattleService {

  constructor(private http: HttpClient) { }

  public postBattleRequest(req: BattleRequest) : Observable<GetResponse>{
    const response = this.http.post(AppSettings.API_URL + 'battle', req) as Observable<GetResponse>;
    return response;
  }

  public postTournamentRequest(req: TournamentRequest) : Observable<PostResponse> {
    const response = this.http.post(AppSettings.API_URL + 'tournament', req, {withCredentials: true}) as Observable<PostResponse>;
    return response;
  }

  public getBattle(id: number) : Observable<GetResponse> {
    const response = this.http.get(AppSettings.API_URL + `battle/${id}`) as Observable<GetResponse>;
    return response;
  }

  public getTournamentResults(id: number) : Observable<GetResponse>{
    const response = this.http.get(AppSettings.API_URL + `tournament/${id}`) as Observable<GetResponse>;
    return response;
  }

  public getTournamentTrainer(id: number) : Observable<GetResponse>{
    const response = this.http.get(AppSettings.API_URL + `trainer/${id}`) as Observable<GetResponse>;
    return response;
  }

}
