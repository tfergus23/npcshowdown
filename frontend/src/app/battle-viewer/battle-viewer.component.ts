import { Component, ElementRef, ViewChild } from '@angular/core';
import { Application, Assets, Point, Sprite, Ticker, Text, ContainerChild } from 'pixi.js';

const HORIZONTAL_PADDING_FACTOR = 4.5;
const POKEMON_SCALE = 2.5;

interface EventLogPokemon{
  species: string,
  maxHealth: number;
  gender: string;
}

interface EventLogTrainer{
  name: string;
  team: Array<EventLogPokemon>
}

interface EventLogEvent{
  type: string;
  message: string;
  data: any;
}

interface BattleEventLog{
  trainer1: EventLogTrainer;
  trainer2: EventLogTrainer;
  events: Array<EventLogEvent>;
}

@Component({
  selector: 'app-battle-viewer',
  templateUrl: './battle-viewer.component.html',
  styleUrls: ['./battle-viewer.component.css']
})
export class BattleViewerComponent {
  @ViewChild('pixiContainer', { static: true }) pixiContainer!: ElementRef;
  battle: BattleEventLog = {
    trainer1: {
      name: 'Guy Dudebro',
      team: [
        {
          species: 'bulbasaur',
          maxHealth: 150,
          gender: "Male"
        }
      ]
    },
    trainer2: {
      name: 'Youngster Joey',
      team: [
        {
          species: 'bulbasaur',
          maxHealth: 150,
          gender: "Female"
        }
      ]
    },
    events: [
      {
        type: 'RANGED_ATTACK',
        message: 'Bulbasaur used Focus Blast!',
        data: {
          attackerIsPlayer1: true
        }
      },
      {
        type: 'MELEE_ATTACK',
        message: 'Bulbasaur used Focus Blast!',
        data: {
          attackerIsPlayer1: false
        }
      }
    ]
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
  projectile: Sprite = new Sprite();
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
          this.app.stage.addChild(this.poke1);

          this.poke2.texture = await Assets.load(`/assets/battle_sprites/${this.determineFileName(this.battle.trainer2.team[0].species)}`);
          this.poke2.anchor.set(0.5);
          this.poke2.scale.x = POKEMON_SCALE;
          this.poke2.scale.y = POKEMON_SCALE;
          this.app.stage.addChild(this.poke2);

          this.projectile.texture = await Assets.load("/assets/battle_sprites/projectile.png");
          this.projectile.anchor.set(0.5);
          this.projectile.scale.x = 0.35;
          this.projectile.scale.y = 0.35;
          this.app.stage.addChild(this.projectile);

          await Assets.load('/assets/Unageo-Bold.ttf');

          this.weatherLabel.anchor.set(0.5);
          this.app.stage.addChild(this.weatherLabel);

          //this.startAnimation(this.poke2MeleeAttack);
          
      
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
  projectileHome: Point = new Point(-2, -2);

  poke1ScreenPos: Point = new Point(this.poke1Home.x, this.poke1Home.y);
  poke2ScreenPos: Point = new Point(this.poke2Home.x, this.poke2Home.y);
  projectilePos: Point = new Point(this.projectileHome.x, this.projectileHome.y);

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

  poke1RangedAttack = new Animation([
    new Tween(this.poke1ScreenPos, new Point(this.poke1ScreenPos.x - 0.025, this.poke1ScreenPos.y), 400),
    new Tween(this.poke1ScreenPos, new Point(this.poke1ScreenPos.x + 0.040, this.poke1ScreenPos.y), 150),
    new Tween(this.poke1ScreenPos, new Point(this.poke1ScreenPos.x, this.poke1ScreenPos.y), 400),
  ]);

  poke2RangedAttack = new Animation([
    new Tween(this.poke2ScreenPos, new Point(this.poke2ScreenPos.x + 0.025, this.poke2ScreenPos.y), 400),
    new Tween(this.poke2ScreenPos, new Point(this.poke2ScreenPos.x - 0.040, this.poke2ScreenPos.y), 150),
    new Tween(this.poke2ScreenPos, new Point(this.poke2ScreenPos.x, this.poke2ScreenPos.y), 400),
  ]);

  poke1Projectile = new Animation([
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 550),
    new Tween(this.projectilePos, new Point(this.poke1ScreenPos.x + 0.040, this.poke1ScreenPos.y), 0),
    new Tween(this.projectilePos, new Point(this.poke2ScreenPos.x, this.poke2ScreenPos.y), 350),
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 0),
  ]);

  poke2Projectile = new Animation([
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 550),
    new Tween(this.projectilePos, new Point(this.poke2ScreenPos.x - 0.040, this.poke2ScreenPos.y), 0),
    new Tween(this.projectilePos, new Point(this.poke1ScreenPos.x, this.poke1ScreenPos.y), 350),
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 0),
  ]);

  currentEvent: number = 0;

  screenToWorldPos(point: Point) : Point{
    return new Point(this.app.screen.width * point.x, this.app.screen.height * point.y);
  }

  update(time: Ticker){
    this.addAnimations();

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
    this.projectile.position = this.screenToWorldPos(this.projectilePos);
  }

  updateAnimations(){
    this.animations.forEach(anim => {
      anim.update(this.tweens);
    });

    this.animations = this.animations.filter(anim => !anim.done);
  }

  startAnimation(anim: Animation){
    let animCopy: Animation = new Animation([]);
    animCopy = Object.assign(animCopy, anim);
    this.animations.push(animCopy);
    animCopy.start(this.tweens);
  }

  addAnimations(){
    if (this.animations.length != 0 || this.currentEvent >= this.battle.events.length) return;
    const event = this.battle.events[this.currentEvent];
    
    switch (event.type){
      case "MELEE_ATTACK":
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1MeleeAttack : this.poke2MeleeAttack);
        break;
      case "RANGED_ATTACK":
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1RangedAttack : this.poke2RangedAttack);
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1Projectile : this.poke2Projectile);
        break;
      default:
        console.error(`Unimplemented event type: ${event.type}`)
    }

    this.currentEvent = (this.currentEvent + 1) % this.battle.events.length;
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
  currentTweenIndex: number  = 0;
  currentTweenRef: Tween;
  done: boolean = false;
  constructor(tweens: Array<Tween>){
    this.tweens = tweens;
    this.currentTweenRef = this.tweens[0];
  }

  start(stageTweens:Array<Tween>){
    this.currentTweenRef = new Tween(new Point(0,0), new Point(0,0), 0);
    Object.assign(this.currentTweenRef, this.tweens[this.currentTweenIndex]);
    stageTweens.push(this.currentTweenRef);
  }

  update(stageTweens:Array<Tween>){
    if (this.done) return;

    if (this.currentTweenRef.done){
      this.currentTweenIndex++;
      if (this.currentTweenIndex >= this.tweens.length){
        this.done = true;
        return;
      }
      this.currentTweenRef = new Tween(new Point(0,0), new Point(0,0), 0);
      Object.assign(this.currentTweenRef, this.tweens[this.currentTweenIndex]);
      stageTweens.push(this.currentTweenRef);
    }
  }
}