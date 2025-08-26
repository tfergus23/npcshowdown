import { Component, CSP_NONCE, ElementRef, ViewChild } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { Application, Assets, Point, Sprite, Ticker, Text, ContainerChild, DEPRECATED_SCALE_MODES, Container } from 'pixi.js';
import { Tween } from './Tween';
import { Animation } from './Animation';
import { BattleEventLog } from './BattleEventLog';
import { POKEMON_SCALE, TEXTBOX_WIDTH, ACTIVE_HEALTHBAR_WIDTH } from './Constants';
import { ActiveHealthBar } from './ActiveHealthBar';
import { PartyHealthBars } from './PartyHealthBars';



@Component({
  selector: 'app-battle-viewer',
  templateUrl: './battle-viewer.component.html',
  styleUrls: ['./battle-viewer.component.css']
})
export class BattleViewerComponent {
  constructor(private router: Router, private activatedRoute: ActivatedRoute){}


  @ViewChild('pixiContainer', { static: true }) pixiContainer!: ElementRef;
  battle: BattleEventLog = {
    trainer1: {
      name: 'Guy Dudebro',
      team: [
        {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Male"
        },
        {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Male"
        },
                {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Male"
        },
                {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Male"
        },
                {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Male"
        },
                {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Male"
        },
      ]
    },
    trainer2: {
      name: 'Youngster Joey',
      team: [
        {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Female"
        }
      ]
    },
    events: [
      {
        type: 'RANGED_ATTACK',
        message: 'Crabominable used All-Out-Pummeling!',
        data: {
          attackerIsPlayer1: true,
          damage: 100
        }
      },
      {
        type: 'MELEE_ATTACK',
        message: 'Bulbasaur used Vine Whip!',
        data: {
          attackerIsPlayer1: false,
          damage: 149
        }
      },
      {
        type: 'RANGED_ATTACK',
        message: 'Bulbasaur used Focus Blast!',
        data: {
          attackerIsPlayer1: true,
          damage: 49
        }
      },
    ]
  };
  trainer1Textures: Array<any> = [];
  trainer2Textures: Array<any> = [];
  trainer1Health: Array<Point> = [];
  trainer2Health: Array<Point> = [];
  poke1: Sprite = new Sprite();
  poke2: Sprite = new Sprite();
  textBoxContaainer = new Container();
  textBox: Sprite = new Sprite();
  textBoxText: Text = new Text({
    text: "",
    style: {
      fontFamily: 'Unageo-Bold',
      fontSize: 25,
      wordWrap: true,
      wordWrapWidth: TEXTBOX_WIDTH,
      align: 'left',
      fill: 'white'
    }
  })
  weatherLabel: Text = new Text({
    text: 'Weather:',
    style: {
      fontFamily: 'Unageo-Bold',
      fontSize: 35,
    }
  });
  weatherText: Text = new Text({
    text: 'Clear',
    style: {
      fontFamily: 'Unageo-Bold',
      fontSize: 35,
    }
  });
  weatherContainer = new Container<ContainerChild>();
  projectile: Sprite = new Sprite();
  poke1HealthBar!: ActiveHealthBar;
  poke2HealthBar!: ActiveHealthBar;
  party1HealthBars!: PartyHealthBars;
  party2HealthBars!: PartyHealthBars;
  app: Application = new Application();
  statusTextures: any = {};

  setTrainer1Poke(index: number){
    this.poke1Health = this.trainer1Health[index];
    this.poke1.texture = this.trainer1Textures[index];
    this.poke1HealthBar.nameText.text = this.battle.trainer1.team[index].species + (this.battle.trainer1.team[index].gender == "Male" ? '♂' : '♀');
  }

  setTrainer2Poke(index: number){
    this.poke2Health = this.trainer2Health[index];
    this.poke2.texture = this.trainer2Textures[index];
    this.poke2HealthBar.nameText.text = this.battle.trainer2.team[index].species + (this.battle.trainer2.team[index].gender == "Male" ? '♂' : '♀');
  }

  ngOnInit(){
    for (let i = 0; i < this.battle.trainer1.team.length; i++){
      const max = this.battle.trainer1.team[i].maxHealth;
      this.trainer1Health[i] = new Point(max, max);
    }
    for (let i = 0; i < this.battle.trainer2.team.length; i++){
      const max = this.battle.trainer2.team[i].maxHealth;
      this.trainer2Health[i] = new Point(max, max);
    }
    this.poke1Health = this.trainer1Health[0];
    this.poke2Health = this.trainer2Health[0];

    setTimeout((async () =>
      {
          // Create a new application

          
        
          // Initialize the application
          await this.app.init({ background: '#8c8c8c', resizeTo: this.pixiContainer.nativeElement });
      
          // Append the application canvas to the document body
          this.pixiContainer.nativeElement.appendChild(this.app.canvas);

          for (let i = 0; i < this.battle.trainer1.team.length; i++){
            this.trainer1Textures[i] = await Assets.load(`/assets/battle_sprites/pokemon/${this.determineFileName(this.battle.trainer1.team[i].species)}`);
          }
          for (let i = 0; i < this.battle.trainer2.team.length; i++){
            this.trainer2Textures[i] = await Assets.load(`/assets/battle_sprites/pokemon/${this.determineFileName(this.battle.trainer2.team[i].species)}`);
          }

          const firstPokemonIndex = 0;
          const trainer1Number = 0;
          const trainer2Number = 1;
          this.poke1HealthBar = new ActiveHealthBar(this.battle, firstPokemonIndex, trainer1Number, this.poke1Health, 
            await Assets.load('/assets/battle_sprites/healthbar_background.png'),
            await Assets.load('/assets/battle_sprites/health.png'),
            await Assets.load('/assets/battle_sprites/missing_health.png')
          );
          this.poke1HealthBar.addToStage(this.app.stage);

          this.poke2HealthBar = new ActiveHealthBar(this.battle, firstPokemonIndex, trainer2Number, this.poke2Health, 
            await Assets.load('/assets/battle_sprites/healthbar_background.png'),
            await Assets.load('/assets/battle_sprites/health.png'),
            await Assets.load('/assets/battle_sprites/missing_health.png')
          );
          this.poke2HealthBar.addToStage(this.app.stage);

          const PARTY_BARS_DISTANCE_FROM_EDGE = -0.017;

          this.party1HealthBars = new PartyHealthBars(
            await Assets.load('/assets/battle_sprites/health.png'),
            await Assets.load('/assets/battle_sprites/missing_health.png'),
            this.trainer1Health,
            this.trainer1Textures,
            new Point(PARTY_BARS_DISTANCE_FROM_EDGE, 0.5),
            1
          );
          this.party1HealthBars.addToStage(this.app.stage);

          this.party2HealthBars = new PartyHealthBars(
            await Assets.load('/assets/battle_sprites/health.png'),
            await Assets.load('/assets/battle_sprites/missing_health.png'),
            this.trainer2Health,
            this.trainer2Textures,
            new Point(1 - PARTY_BARS_DISTANCE_FROM_EDGE, 0.5),
            2
          );
          this.party2HealthBars.addToStage(this.app.stage);

          this.setTrainer1Poke(0);
          this.poke1.anchor.set(0.5);
          this.poke1.scale.x = -POKEMON_SCALE;
          this.poke1.scale.y = POKEMON_SCALE;
          this.app.stage.addChild(this.poke1);

          this.setTrainer2Poke(0);
          this.poke2.anchor.set(0.5);
          this.poke2.scale.x = POKEMON_SCALE;
          this.poke2.scale.y = POKEMON_SCALE;
          this.app.stage.addChild(this.poke2);

          this.projectile.texture = await Assets.load("/assets/battle_sprites/projectile.png");
          this.projectile.anchor.set(0.5);
          this.projectile.scale.x = 0.35;
          this.projectile.scale.y = 0.35;
          this.app.stage.addChild(this.projectile);

          this.textBox.texture = await Assets.load("/assets/battle_sprites/textbox.png");
          this.textBox.width = TEXTBOX_WIDTH;
          this.textBox.anchor.set(0.5);
          this.app.stage.addChild(this.textBox);

          await Assets.load('/assets/Unageo-Bold.ttf');

          const WEAHTER_TEXT_VERTICAL_DISTANCE = 20;

          this.weatherLabel.anchor.set(0.5);
          this.weatherLabel.position.y = -WEAHTER_TEXT_VERTICAL_DISTANCE;
          this.weatherContainer.addChild(this.weatherLabel);

          this.weatherText.anchor.set(0.5);
          this.weatherText.position.y = WEAHTER_TEXT_VERTICAL_DISTANCE;
          this.weatherContainer.addChild(this.weatherText);

          this.app.stage.addChild(this.weatherContainer);

          this.textBoxText.anchor.set(0.5);
          this.app.stage.addChild(this.textBoxText);

          this.statusTextures['Poison'] = await Assets.load('/assets/battle_sprites/status/poison.png');
          this.statusTextures['Paralysis'] = await Assets.load('/assets/battle_sprites/status/paralysis.png');
          this.statusTextures['Sleep'] = await Assets.load('/assets/battle_sprites/status/sleep.png');
          this.statusTextures['Burn'] = await Assets.load('/assets/battle_sprites/status/burn.png');
          this.statusTextures['Freeze'] = await Assets.load('/assets/battle_sprites/status/freeze.png');
          this.statusTextures['Toxic'] = await Assets.load('/assets/battle_sprites/status/toxic.png');

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

  weatherContainerScreenPos: Point = new Point(0.5, 0.1);
  tweens: Array<Tween> = new Array<Tween>();
  animations: Array<Animation> = new Array<Animation>();

  poke1Home: Point = new Point(0.30, 0.5);
  poke2Home: Point = new Point(0.70, 0.5);
  projectileHome: Point = new Point(-2, -2);

  poke1ScreenPos: Point = new Point(this.poke1Home.x, this.poke1Home.y);
  poke2ScreenPos: Point = new Point(this.poke2Home.x, this.poke2Home.y);
  projectilePos: Point = new Point(this.projectileHome.x, this.projectileHome.y);
  textBoxTextScreenPos: Point = new Point(0.5, 0.70);
  poke1HealthScreenPos: Point = new Point(this.poke1ScreenPos.x, this.poke1ScreenPos.y - 0.20);
  poke2HealthScreenPos: Point = new Point(this.poke2ScreenPos.x, this.poke2ScreenPos.y - 0.20);

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
  totalTimeMS: number = 0;
  update(time: Ticker){

    this.updateCurrentEvent(time.deltaMS);

    this.poke1HealthBar.update();
    this.poke2HealthBar.update();
    this.party1HealthBars.update();
    this.party2HealthBars.update();

    this.updateTweens(time.deltaMS);
    
    this.updateAnimations();
    this.updateTextScroll(time.deltaMS);

    this.updateWorldPositions();
    this.totalTimeMS += time.deltaMS;
  }

  poke1Health!: Point;
  poke2Health!: Point;

  currentText: string = this.battle.events[this.currentEvent].message;
  nextCharTime: number = 0;
  updateTextScroll(deltaMS: number){
    const TIME_BETWEEN_CHARS_MS = 20;
    if (this.textBoxText.text.length < this.currentText.length && this.totalTimeMS >= this.nextCharTime){
      this.textBoxText.text += this.currentText[this.textBoxText.text.length];
      this.nextCharTime = this.totalTimeMS + TIME_BETWEEN_CHARS_MS;
    }
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
    this.weatherContainer.position = this.screenToWorldPos(this.weatherContainerScreenPos);
    this.projectile.position = this.screenToWorldPos(this.projectilePos);
    this.textBox.position = this.screenToWorldPos(this.textBoxTextScreenPos);
    this.textBoxText.position = this.screenToWorldPos(this.textBoxTextScreenPos);
    this.poke1HealthBar.container.position = this.screenToWorldPos(this.poke1HealthScreenPos);
    this.poke2HealthBar.container.position = this.screenToWorldPos(this.poke2HealthScreenPos);
    this.party1HealthBars.container.position = this.screenToWorldPos(this.party1HealthBars.screenSpacePos);
    this.party2HealthBars.container.position = this.screenToWorldPos(this.party2HealthBars.screenSpacePos);
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

  clamp(num: number, min: number, max: number) {
    return Math.min(Math.max(num, min), max);
  }

  startHealthTween(currentHealth: Point, healthChange: number, delayMS: number){
    const HEALTH_MOVE_TIME_MS = 350;
    let anim = new Animation([
      new Tween(currentHealth, currentHealth, delayMS),
      new Tween(currentHealth, new Point(this.clamp(currentHealth.x + healthChange, 0, currentHealth.y), currentHealth.y), HEALTH_MOVE_TIME_MS),
    ]);
    this.startAnimation(anim);
  }

  updateCurrentEvent(dt: number){
    if (this.textBoxText.text.length < this.currentText.length){
      this.updateTextScroll(dt);
      if (this.textBoxText.text.length == this.currentText.length){
        this.addAnimations();
      }
    }
    else if (this.animations.length != 0){

    }
    else if (this.currentEvent < this.battle.events.length-1) {
      this.currentEvent++;
      this.textBoxText.text = "";
      console.log(this.currentEvent);
      this.currentText = this.battle.events[this.currentEvent].message;
    }
  }

  addAnimations(){
    if (this.animations.length != 0 || this.currentEvent >= this.battle.events.length) return;
    const event = this.battle.events[this.currentEvent];
    
    switch (event.type){
      case "MELEE_ATTACK":
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1MeleeAttack : this.poke2MeleeAttack);
        this.startHealthTween(event.data.attackerIsPlayer1 ? this.poke2Health : this.poke1Health, -event.data.damage, 1350);
        break;
      case "RANGED_ATTACK":
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1RangedAttack : this.poke2RangedAttack);
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1Projectile : this.poke2Projectile);
        this.startHealthTween(event.data.attackerIsPlayer1 ? this.poke2Health : this.poke1Health, -event.data.damage, 900);
        break;
      default:
        console.error(`Unimplemented event type: ${event.type}`)
    }
  }
}

