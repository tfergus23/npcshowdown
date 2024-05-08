import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DataListsComponent } from './data-lists.component';

describe('DataListsComponent', () => {
  let component: DataListsComponent;
  let fixture: ComponentFixture<DataListsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DataListsComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DataListsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
