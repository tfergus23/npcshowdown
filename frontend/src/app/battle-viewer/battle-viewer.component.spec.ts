import { ComponentFixture, TestBed } from '@angular/core/testing';

import { BattleViewerComponent } from './battle-viewer.component';

describe('BattleViewerComponent', () => {
  let component: BattleViewerComponent;
  let fixture: ComponentFixture<BattleViewerComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [BattleViewerComponent]
    });
    fixture = TestBed.createComponent(BattleViewerComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
