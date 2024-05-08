import { Component, Input } from '@angular/core';
import DataLists from 'src/DataLists';

@Component({
  selector: 'app-data-lists',
  templateUrl: './data-lists.component.html',
  styleUrls: ['./data-lists.component.css']
})
export class DataListsComponent {
  @Input() dataLists?: DataLists;
}
