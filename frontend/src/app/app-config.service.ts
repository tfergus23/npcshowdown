import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import AppConfig from 'src/AppConfig';

@Injectable({
  providedIn: 'root'
})
export class AppConfigService {

  constructor(private http: HttpClient) { }
  private appConfig?: AppConfig;

  loadAppConfig(): Promise<void> {
    return this.http
      .get<AppConfig>('/assets/AppConfig.json')
      .toPromise()
      .then((config) => {
        this.appConfig = config;
      });
  }

  getConfig(): AppConfig {
    return this.appConfig!;
  }
}
