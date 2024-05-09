import { ComponentFixture, TestBed } from '@angular/core/testing';

import { BattleLogViewComponent } from './battle-log-view.component';

describe('BattleLogViewComponent', () => {
  let component: BattleLogViewComponent;
  let fixture: ComponentFixture<BattleLogViewComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ BattleLogViewComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(BattleLogViewComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
