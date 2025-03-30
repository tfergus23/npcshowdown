import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import BattleRequest from 'src/BattleRequest';
import GetResponse from 'src/GetResponse';
import PostResponse from 'src/PostResponse';
import TournamentRequest from 'src/TournamentRequest';
import { TournamentResultSet } from 'src/TournamentResultSet';
import { AppConfigService } from './app-config.service';

@Injectable({
  providedIn: 'root'
})
export class BattleService {

  constructor(private http: HttpClient, private config: AppConfigService) { }

  public postBattleRequest(req: BattleRequest) : Observable<GetResponse>{
    const response = this.http.post(this.config.getConfig().apiURL + 'battle', req) as Observable<GetResponse>;
    return response;
  }

  public postTournamentRequest(req: TournamentRequest) : Observable<PostResponse> {
    const response = this.http.post(this.config.getConfig().apiURL + 'tournament', req, {withCredentials: true}) as Observable<PostResponse>;
    return response;
  }

  public getTournamentResults(id: number) : Observable<GetResponse>{
    const response = this.http.get(this.config.getConfig().apiURL + `tournament/${id}`) as Observable<GetResponse>;
    return response;
  }

  public getTournamentTrainer(id: number) : Observable<GetResponse>{
    const response = this.http.get(this.config.getConfig().apiURL + `trainer/${id}`) as Observable<GetResponse>;
    return response;
  }

}
