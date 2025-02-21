import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UserTrainersModalComponent } from './user-trainers-modal.component';

describe('UserTrainersModalComponent', () => {
  let component: UserTrainersModalComponent;
  let fixture: ComponentFixture<UserTrainersModalComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ UserTrainersModalComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(UserTrainersModalComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
