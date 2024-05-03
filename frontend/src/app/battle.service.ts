import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';
import BattleRequest from 'src/BattleRequest';
import PostResponse from 'src/PostResponse';
import TournamentRequest from 'src/TournamentRequest';

@Injectable({
  providedIn: 'root'
})
export class BattleService {

  constructor(private http: HttpClient) { }

  public postBattleRequest(req: BattleRequest) : Observable<PostResponse>{
    const response = this.http.post(AppSettings.API_URL + 'battle', req) as Observable<PostResponse>;
    return response;
  }

  public postTournamentRequest(req: TournamentRequest) : Observable<PostResponse> {
    const response = this.http.post(AppSettings.API_URL + 'tournament', req) as Observable<PostResponse>;
    return response;
  }
}
