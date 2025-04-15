import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SpinningButtonComponent } from './spinning-button.component';

describe('SpinningButtonComponent', () => {
  let component: SpinningButtonComponent;
  let fixture: ComponentFixture<SpinningButtonComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SpinningButtonComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(SpinningButtonComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
