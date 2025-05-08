import { Component, ElementRef, ViewChild } from '@angular/core';
import { Application, Assets, Point, Sprite, Ticker, Text, ContainerChild } from 'pixi.js';

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
  weatherLabel: Text = new Text({
    text: 'Weather:',
    style: {
      fontFamily: 'Unageo-Bold',
      fontSize: 35
    }
  });
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

          await Assets.load('/assets/Unageo-Bold.ttf');

          this.weatherLabel.anchor.set(0.5);
          this.app.stage.addChild(this.weatherLabel);

          this.startAnimation(this.poke2MeleeAttack);
          
      
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

  weatherLabelScreenPos: Point = new Point(0.5, 0.1);
  tweens: Array<Tween> = new Array<Tween>();
  animations: Array<Animation> = new Array<Animation>();

  poke1Home: Point = new Point(0.25, 0.5);
  poke2Home: Point = new Point(0.75, 0.5);

  poke1ScreenPos: Point = new Point(this.poke1Home.x, this.poke1Home.y);
  poke2ScreenPos: Point = new Point(this.poke2Home.x, this.poke2Home.y);

  poke1MeleeAttack = new Animation([
    new Tween(this.poke1ScreenPos, new Point(this.poke2ScreenPos.x - 0.10, this.poke2ScreenPos.y), 700),
    new Tween(this.poke1ScreenPos, new Point(this.poke2ScreenPos.x - 0.15, this.poke2ScreenPos.y), 500),
    new Tween(this.poke1ScreenPos, new Point(this.poke2ScreenPos.x, this.poke2ScreenPos.y), 150),
    new Tween(this.poke1ScreenPos, new Point(this.poke2ScreenPos.x - 0.15, this.poke2ScreenPos.y), 150),
    new Tween(this.poke1ScreenPos, new Point(this.poke2ScreenPos.x - 0.15, this.poke2ScreenPos.y), 50),
    new Tween(this.poke1ScreenPos, this.poke1Home, 700),
  ]);

  poke2MeleeAttack = new Animation([
    new Tween(this.poke2ScreenPos, new Point(this.poke1ScreenPos.x + 0.10, this.poke1ScreenPos.y), 700),
    new Tween(this.poke2ScreenPos, new Point(this.poke1ScreenPos.x + 0.15, this.poke1ScreenPos.y), 500),
    new Tween(this.poke2ScreenPos, new Point(this.poke1ScreenPos.x, this.poke1ScreenPos.y), 150),
    new Tween(this.poke2ScreenPos, new Point(this.poke1ScreenPos.x + 0.15, this.poke1ScreenPos.y), 150),
    new Tween(this.poke2ScreenPos, new Point(this.poke1ScreenPos.x + 0.15, this.poke1ScreenPos.y), 50),
    new Tween(this.poke2ScreenPos, this.poke2Home, 700),
  ]);

  screenToWorldPos(point: Point) : Point{
    return new Point(this.app.screen.width * point.x, this.app.screen.height * point.y);
  }

  update(time: Ticker){
    this.updateTweens(time.deltaMS);
    this.updateAnimations();

    this.updateWorldPositions();
  }

  updateTweens(dt: number){
    this.tweens.forEach((tween, i) =>{
      tween.update(dt);
    });
    this.tweens = this.tweens.filter(tween => !tween.done);
  }

  updateWorldPositions(){
    this.poke1.position = this.screenToWorldPos(this.poke1ScreenPos);
    this.poke2.position = this.screenToWorldPos(this.poke2ScreenPos);
    this.weatherLabel.position = this.screenToWorldPos(this.weatherLabelScreenPos);
  }

  updateAnimations(){
    this.animations.forEach(anim => {
      anim.update(this.tweens);
    });

    this.animations = this.animations.filter(anim => !anim.done);
  }

  startAnimation(anim: Animation){
    this.animations.push(anim);
    anim.start(this.tweens);
  }
}

class Tween{
  object: Point;
  start: Point;
  dest: Point;
  elapsed: number = 0.0;
  totalTime: number;
  done: boolean = false;
  started: boolean = false;
  constructor(object: Point, dest: Point, time: number){
    this.object = object;
    this.start = new Point(object.x ,object.y);
    this.dest = dest;
    this.totalTime = time;
  }

  update(dt: number){
    if (!this.started){
      this.start = new Point(this.object.x, this.object.y);
      this.started = true;
    }
    if (this.done) return;

    this.elapsed += dt;
    
    if (this.elapsed >= this.totalTime){
      this.object.x = this.dest.x;
      this.object.y = this.dest.y;
      this.done = true;
      return;
    }
    let fullLength: Point = new Point(this.dest.x - this.start.x, this.dest.y - this.start.y);
    let percentTraveled = this.elapsed / this.totalTime;
    let distanceTraveled: Point = new Point(fullLength.x * percentTraveled, fullLength.y * percentTraveled);
    this.object.x = this.start.x + distanceTraveled.x;
    this.object.y = this.start.y + distanceTraveled.y;
  }
}

class Animation{
  tweens: Array<Tween>;
  currentTween: number  = 0;
  done: boolean = false;
  constructor(tweens: Array<Tween>){
    this.tweens = tweens;
  }

  start(stageTweens:Array<Tween>){
    stageTweens.push(this.tweens[this.currentTween]);
  }

  update(stageTweens:Array<Tween>){
    if (this.done) return;

    if (this.tweens[this.currentTween].done){
      this.currentTween++;
      if (this.currentTween >= this.tweens.length){
        this.done = true;
        return;
      }
      stageTweens.push(this.tweens[this.currentTween]);
    }
  }
}