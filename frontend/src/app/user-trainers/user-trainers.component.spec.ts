import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UserTrainersComponent } from './user-trainers.component';

describe('UserTrainersComponent', () => {
  let component: UserTrainersComponent;
  let fixture: ComponentFixture<UserTrainersComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ UserTrainersComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(UserTrainersComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
