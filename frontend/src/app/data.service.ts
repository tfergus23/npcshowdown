import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import DataLists from 'src/DataLists';
import GetResponse from 'src/GetResponse';
import { AppConfigService } from './app-config.service';

@Injectable({
  providedIn: 'root'
})
export class DataService {

  constructor(private http: HttpClient, private config: AppConfigService) { }

  getAllData() : Observable<GetResponse>{
    const response = this.http.get(`${this.config.getConfig().apiURL}data`) as Observable<GetResponse>;
    return response;
  }
}
