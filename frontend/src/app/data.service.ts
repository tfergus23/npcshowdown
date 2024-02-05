import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
import { AppSettings } from 'src/AppSettings';
import DataLists from 'src/DataLists';
import GetResponse from 'src/GetResponse';

@Injectable({
  providedIn: 'root'
})
export class DataService {

  constructor(private http: HttpClient) { }

  getAllData() : Observable<GetResponse>{
    const response = this.http.get(`${AppSettings.API_URL}data`) as Observable<GetResponse>;
    return response;
  }
}
