import { Component, ElementRef, ViewChild } from '@angular/core';
import { Application, Assets, Point, Sprite, Ticker } from 'pixi.js';

const HORIZONTAL_PADDING_FACTOR = 4.5;
const POKEMON_SCALE = 2.5;

@Component({
  selector: 'app-battle-viewer',
  templateUrl: './battle-viewer.component.html',
  styleUrls: ['./battle-viewer.component.css']
})
export class BattleViewerComponent {
  @ViewChild('pixiContainer', { static: true }) pixiContainer!: ElementRef;
  battle: any = {
    trainer1: {
      name: 'Guy Dudebro',
      team: [
        {
          species: 'bulbasaur',
          maxHealth: 150
        }
      ]
    },
    trainer2: {
      name: 'Youngster Joey',
      team: [
        {
          species: 'bulbasaur',
          maxHealth: 150
        }
      ]
    }
  };
  trainer1Textures: Array<any> = [];
  trainer2Textures: Array<any> = [];
  poke1: Sprite = new Sprite();
  poke2: Sprite = new Sprite();
  app: Application = new Application();

  ngOnInit(){
    setTimeout((async () =>
      {
          // Create a new application

          
        
          // Initialize the application
          await this.app.init({ background: '#8c8c8c', resizeTo: this.pixiContainer.nativeElement });
      
          // Append the application canvas to the document body
          this.pixiContainer.nativeElement.appendChild(this.app.canvas);
      
          /*
          const texture = await Assets.load('https://pixijs.com/assets/bunny.png');
          const bunny = new Sprite(texture);
          bunny.anchor.set(0.5);
          app.stage.addChild(bunny);
          */

          this.poke1.texture = await Assets.load(`/assets/battle_sprites/${this.determineFileName(this.battle.trainer1.team[0].species)}`);
          this.poke1.anchor.set(0.5);
          this.poke1.scale.x = -POKEMON_SCALE;
          this.poke1.scale.y = POKEMON_SCALE;
          this.poke2.texture = await Assets.load(`/assets/battle_sprites/${this.determineFileName(this.battle.trainer2.team[0].species)}`);
          this.poke2.anchor.set(0.5);
          this.poke2.scale.x = POKEMON_SCALE;
          this.poke2.scale.y = POKEMON_SCALE;
          this.app.stage.addChild(this.poke1);
          this.app.stage.addChild(this.poke2);

          this.poke1.y = this.app.screen.height / 2;
      
          // Listen for animate update
          this.app.ticker.add((time) =>
          {
            this.update(time);
          });
      }));
  }

  determineFileName(poke: string) : string{
    if (poke == 'Type: Null'){
      return 'type_null.png';
    }
    return poke.toLowerCase() + ".png";
  }

  poke1ScreenPos: Point = new Point(0.25, 0.5);
  poke2ScreenPos: Point = new Point(0.75, 0.5);

  update(time: Ticker){
    this.poke1.position = new Point(this.app.screen.width * this.poke1ScreenPos.x, this.app.screen.height * this.poke1ScreenPos.y);
    this.poke2.position = new Point(this.app.screen.width * this.poke2ScreenPos.x, this.app.screen.height * this.poke2ScreenPos.y);
  }
}