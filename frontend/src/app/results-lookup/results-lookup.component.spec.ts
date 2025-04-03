import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ResultsLookupComponent } from './results-lookup.component';

describe('ResultsLookupComponent', () => {
  let component: ResultsLookupComponent;
  let fixture: ComponentFixture<ResultsLookupComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ResultsLookupComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ResultsLookupComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
