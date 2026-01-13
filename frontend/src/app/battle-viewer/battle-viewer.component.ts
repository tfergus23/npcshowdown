import { Component, CSP_NONCE, ElementRef, ViewChild } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { Application, Assets, Point, Sprite, Ticker, Text, ContainerChild, DEPRECATED_SCALE_MODES, Container, Pool } from 'pixi.js';
import { Tween } from './Tween';
import { Animation } from './Animation';
import { BattleEventLog } from './BattleEventLog';
import { POKEMON_SCALE, TEXTBOX_WIDTH, ACTIVE_HEALTHBAR_WIDTH, DEFAULT_BACKGROUND_COLOR, SUN_BACKGROUND_COLOR, SANDSTORM_BACKGROUND_COLOR, RAIN_BACKGROUND_COLOR, HAIL_BACKGROUND_COLOR } from './Constants';
import { ActiveHealthBar } from './ActiveHealthBar';
import { PartyHealthBars } from './PartyHealthBars';
import { FieldEffectsText } from './FieldEffectsText';
import { BattleService } from '../battle.service';
import BattleRequest from 'src/BattleRequest';

/*
  TODO:
  - Status on switch
  - Clear volatiles on switch
  - Damage delay on attacks
  - Improve resize behavior
  - Fixed delay after animations
  - Increase textbox width
  - Stat boost display
  - Trainer name display
  - Fix the really big pokemon
  - Seed display with easy copy
*/

type Weather = 'Sun' | 'Rain' | 'Hail' | 'Sandstorm' | 'Clear';

@Component({
  selector: 'app-battle-viewer',
  templateUrl: './battle-viewer.component.html',
  styleUrls: ['./battle-viewer.component.css']
})
export class BattleViewerComponent {
  constructor(private router: Router, private activatedRoute: ActivatedRoute, private battleService: BattleService){}


  @ViewChild('pixiContainer', { static: true }) pixiContainer!: ElementRef;
  battle: BattleEventLog = {
    trainer1: {
      name: 'Guy Dudebro',
      team: [
        {
          species: 'Tapu Koko',
          maxHealth: 150,
          gender: "Male",
          name: 'Tapu Koko'
        },
        {
          species: 'Dialga',
          maxHealth: 250,
          gender: "Genderless",
          name: 'Dialga'
        },
      ]
    },
    trainer2: {
      name: 'Youngster Joey',
      team: [
        {
          species: 'Bulbasaur',
          maxHealth: 150,
          gender: "Female",
          name: 'Bulbasaur'
        }
      ]
    },
    events: [
      {
        type: 'RANGED_ATTACK',
        message: 'Ho-oh used All-Out-Pummeling!',
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
          damage: 150
        }
      },
      {
        type: 'POKEMON_FAINT',
        message: 'Ho-oh fainted!',
        data: {
          isPlayer1: true,
        }
      },
      {
        type: 'POKEMON_ENTER',
        message: 'Go Dialga!',
        data: {
          isPlayer1: true,
          newPokeIndex: 1
        }
      },
      {
        type: 'DAMAGE_TAKEN',
        message: 'Dialga is hurt by the stealthy stealth rocks.',
        data: {
          recipientIsPlayer1: true,
          damage: 20
        }
      },
      {
        type: 'DEBUG_MESSAGE',
        message: 'This is a debug message.',
        data: {}
      },
      {
        type: 'HEALING_RECEIVED',
        message: 'Dialga ate some snackies.',
        data: {
          recipientIsPlayer1: true,
          healing: 20
        }
      },
      {
        type: 'WEATHER_CHANGE',
        message: 'The sunlight became harsh!',
        data: {
          weather: 'Sun'
        }
      },
      {
        type: 'APPLY_VOLATILE',
        message: 'Dialga contracted heatstroke!',
        data: {
          appliedToPlayer1: true,
          effect: 'Heatstroke'
        }
      },
      {
        type: 'REMOVE_VOLATILE',
        message: 'Paula used the wet towel!',
        data: {
          removedFromPlayer1: true,
          effect: 'Heatstroke'
        }
      },
    ]
  };
  poke1Home: Point = new Point(0.30, 0.5);
  poke2Home: Point = new Point(0.70, 0.5);
  trainer1Textures: Array<any> = [];
  trainer2Textures: Array<any> = [];
  trainer1Health: Array<Point> = [];
  trainer2Health: Array<Point> = [];
  poke1: Sprite = new Sprite();
  poke2: Sprite = new Sprite();
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
  player1FieldEffects: FieldEffectsText = new FieldEffectsText(new Point(this.poke1Home.x,0.6));
  player2FieldEffects: FieldEffectsText = new FieldEffectsText(new Point(this.poke2Home.x,0.6));
  delayPoint: Point = new Point(); // Quick and dirty way to add delay


  genderSymbol(gender: string){
    if (gender == "Male")
      return '♂';
    else if (gender == "Female"){
      return '♀';
    }
    return '';
  }

  setTrainer1Poke(index: number){
    this.poke1Health = this.trainer1Health[index];
    this.poke1.texture = this.trainer1Textures[index];
    this.poke1.visible = true;
    this.poke1HealthBar.nameText.text = this.battle.trainer1.team[index].name + this.genderSymbol(this.battle.trainer1.team[index].gender);
    this.poke1HealthBar.health = this.poke1Health;
  }

  setTrainer2Poke(index: number){
    this.poke2Health = this.trainer2Health[index];
    this.poke2.texture = this.trainer2Textures[index];
    this.poke2.visible = true;
    this.poke2HealthBar.nameText.text = this.battle.trainer2.team[index].name + this.genderSymbol(this.battle.trainer2.team[index].gender);
    this.poke2HealthBar.health = this.poke2Health;
  }



  ngOnInit(){
    this.activatedRoute.queryParams.subscribe(params => {
      let battleJson: BattleRequest = JSON.parse(decodeURIComponent(params['battle']));
      this.battleService.postBattleRequest(battleJson).subscribe(async (res) =>
      {
          this.battle = res.data;
          this.currentText = this.battle.events[this.currentEvent].message;
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
          
        
          // Initialize the application
          await this.app.init({ background: DEFAULT_BACKGROUND_COLOR, resizeTo: this.pixiContainer.nativeElement });
      
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
          this.statusTextures['Frozen'] = await Assets.load('/assets/battle_sprites/status/frozen.png');
          this.statusTextures['Toxic'] = await Assets.load('/assets/battle_sprites/status/toxic.png');

          this.player1FieldEffects.addToStage(this.app.stage);
          this.player2FieldEffects.addToStage(this.app.stage);

          this.app.renderer.on('resize', (width, height) => this.resizeCallback(width, height));

          this.app.ticker.add((time) =>
          {
            this.update(time);
          });

          setTimeout(() => this.resizeCallback(this.app.canvas.width, this.app.canvas.height));
      },
      (error) =>{

      });
    });
    //setTimeout(());
  }

  resizeCallback(width: number, height: number){
    console.log(`Resized to Width: ${width}, Height: ${height}`);

    if (width < 1350){
      this.party1HealthBars.disable();
      this.party2HealthBars.disable();
    }
    else{
      this.party1HealthBars.enable();
      this.party2HealthBars.enable();
    }

    if (width < 765){
      this.poke1HealthBar.scale(0.5);
      this.poke2HealthBar.scale(0.5);
      this.poke1.scale.set(-1.5, 1.5);
      this.poke2.scale.set(1.5);
      this.textBox.scale.set(0.75);
      this.textBoxText.scale.set(0.75);
    }
    else{
      this.poke1HealthBar.scale(1.0);
      this.poke2HealthBar.scale(1.0);
      this.poke1.scale.set(-POKEMON_SCALE, POKEMON_SCALE);
      this.poke2.scale.set(POKEMON_SCALE);
      this.textBox.scale.set(1);
      this.textBoxText.scale.set(1);
    }

    if (width < 390){
      this.poke1HealthBar.scale(0.40);
      this.poke2HealthBar.scale(0.40);
      this.poke1.scale.set(-1.25, 1.25);
      this.poke2.scale.set(1.25);
    }
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

  projectileHome: Point = new Point(-2, -2);

  poke1ScreenPos: Point = new Point(-0.25, this.poke1Home.y);
  poke2ScreenPos: Point = new Point(1.25, this.poke2Home.y);
  projectilePos: Point = new Point(this.projectileHome.x, this.projectileHome.y);
  textBoxTextScreenPos: Point = new Point(0.5, 0.75);
  poke1HealthScreenPos: Point = new Point(this.poke1Home.x, this.poke1Home.y - 0.20);
  poke2HealthScreenPos: Point = new Point(this.poke2Home.x, this.poke2Home.y - 0.20);

  poke1MeleeAttack = new Animation([
    new Tween(this.poke1ScreenPos, new Point(this.poke2Home.x - 0.10, this.poke2Home.y), 700),
    new Tween(this.poke1ScreenPos, new Point(this.poke2Home.x - 0.15, this.poke2Home.y), 500),
    new Tween(this.poke1ScreenPos, new Point(this.poke2Home.x, this.poke2Home.y), 150),
    new Tween(this.poke1ScreenPos, new Point(this.poke2Home.x - 0.15, this.poke2Home.y), 150),
    new Tween(this.poke1ScreenPos, new Point(this.poke2Home.x - 0.15, this.poke2Home.y), 50),
    new Tween(this.poke1ScreenPos, this.poke1Home, 700),
  ]);

  poke2MeleeAttack = new Animation([
    new Tween(this.poke2ScreenPos, new Point(this.poke1Home.x + 0.10, this.poke1Home.y), 700),
    new Tween(this.poke2ScreenPos, new Point(this.poke1Home.x + 0.15, this.poke1Home.y), 500),
    new Tween(this.poke2ScreenPos, new Point(this.poke1Home.x, this.poke1Home.y), 150),
    new Tween(this.poke2ScreenPos, new Point(this.poke1Home.x + 0.15, this.poke1Home.y), 150),
    new Tween(this.poke2ScreenPos, new Point(this.poke1Home.x + 0.15, this.poke1Home.y), 50),
    new Tween(this.poke2ScreenPos, this.poke2Home, 700),
  ]);

  poke1RangedAttack = new Animation([
    new Tween(this.poke1ScreenPos, new Point(this.poke1Home.x - 0.025, this.poke1Home.y), 400),
    new Tween(this.poke1ScreenPos, new Point(this.poke1Home.x + 0.040, this.poke1Home.y), 150),
    new Tween(this.poke1ScreenPos, new Point(this.poke1Home.x, this.poke1Home.y), 400),
  ]);

  poke2RangedAttack = new Animation([
    new Tween(this.poke2ScreenPos, new Point(this.poke2Home.x + 0.025, this.poke2Home.y), 400),
    new Tween(this.poke2ScreenPos, new Point(this.poke2Home.x - 0.040, this.poke2Home.y), 150),
    new Tween(this.poke2ScreenPos, new Point(this.poke2Home.x, this.poke2Home.y), 400),
  ]);

  poke1Projectile = new Animation([
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 550),
    new Tween(this.projectilePos, new Point(this.poke1Home.x + 0.040, this.poke1Home.y), 0),
    new Tween(this.projectilePos, new Point(this.poke2Home.x, this.poke2Home.y), 350),
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 0),
  ]);

  poke2Projectile = new Animation([
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 550),
    new Tween(this.projectilePos, new Point(this.poke2Home.x - 0.040, this.poke2Home.y), 0),
    new Tween(this.projectilePos, new Point(this.poke1Home.x, this.poke1Home.y), 350),
    new Tween(this.projectilePos, new Point(this.projectileHome.x, this.projectileHome.y), 0),
  ]);

  poke1Leave = new Animation([
    new Tween(this.poke1ScreenPos, new Point(-0.25, this.poke2ScreenPos.y), 700),
  ]);
  poke1Enter = new Animation([
    new Tween(this.poke1ScreenPos, new Point(this.poke1Home.x, this.poke2ScreenPos.y), 700),
  ]);
  poke2Leave = new Animation([
    new Tween(this.poke2ScreenPos, new Point(1.25, this.poke2ScreenPos.y), 700),
  ]);
  poke2Enter = new Animation([
    new Tween(this.poke2ScreenPos, new Point(this.poke2Home.x, this.poke2ScreenPos.y), 700),
  ]);

  currentEvent: number = 0;

  screenToWorldPos(point: Point) : Point{
    return new Point(this.app.screen.width * point.x, this.app.screen.height * point.y);
  }
  totalTimeMS: number = 0;
  timeScale: number = 2.0;
  update(time: Ticker){

    let dt: number = time.deltaMS * this.timeScale;

    this.updateCurrentEvent(dt);

    this.poke1HealthBar.update();
    this.poke2HealthBar.update();
    this.party1HealthBars.update();
    this.party2HealthBars.update();

    this.updateTweens(dt);
    
    this.updateAnimations();
    this.updateTextScroll(dt);

    this.updateWorldPositions();
    this.totalTimeMS += dt;
  }

  poke1Health!: Point;
  poke2Health!: Point;

  currentText: string = "";
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
    this.player1FieldEffects.text.position = this.screenToWorldPos(this.player1FieldEffects.screenSpacePos);
    this.player2FieldEffects.text.position = this.screenToWorldPos(this.player2FieldEffects.screenSpacePos);
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

  beginDelay(delayMS: number){
    let delayAnim = new Animation([
      new Tween(this.delayPoint, this.delayPoint, delayMS),
    ]);
    this.startAnimation(delayAnim);
  }

  clamp(num: number, min: number, max: number) {
    return Math.min(Math.max(num, min), max);
  }

  setWeather(weather: Weather){
    this.weatherText.text = weather;
    switch (weather){
      case 'Clear':
        this.app.renderer.background.color = DEFAULT_BACKGROUND_COLOR;
        break;
      case 'Sun':
        this.app.renderer.background.color = SUN_BACKGROUND_COLOR;
        break;
      case 'Rain':
        this.app.renderer.background.color = RAIN_BACKGROUND_COLOR;
        break;
      case 'Hail':
        this.app.renderer.background.color = HAIL_BACKGROUND_COLOR;
        break;
      case 'Sandstorm':
        this.app.renderer.background.color = SANDSTORM_BACKGROUND_COLOR;
        break;
    }
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
      case 'MESSAGE':
        this.beginDelay(1000);
        break;
      case "MELEE_ATTACK":
        console.log(event.data.attackerIsPlayer1);
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1MeleeAttack : this.poke2MeleeAttack);
        this.startHealthTween(event.data.attackerIsPlayer1 ? this.poke2Health : this.poke1Health, -event.data.damage, 1350);
        this.beginDelay(2700);
        break;
      case "RANGED_ATTACK":
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1RangedAttack : this.poke2RangedAttack);
        this.startAnimation(event.data.attackerIsPlayer1 ? this.poke1Projectile : this.poke2Projectile);
        this.startHealthTween(event.data.attackerIsPlayer1 ? this.poke2Health : this.poke1Health, -event.data.damage, 900);
        this.beginDelay(1500);
        break;
      case 'DAMAGE_TAKEN':
        this.startHealthTween(event.data.recipientIsPlayer1 ? this.poke1Health : this.poke2Health, -event.data.damage, 0);
        this.beginDelay(1200);
        break;
      case 'HEALING_RECEIVED':
        this.startHealthTween(event.data.recipientIsPlayer1 ? this.poke1Health : this.poke2Health, event.data.healing, 0);
        this.beginDelay(1200);
        break;
      case 'POKEMON_ENTER':{
        let newIndex = event.data.newPokeIndex;
        if (event.data.isPlayer1){
          this.setTrainer1Poke(newIndex);
          this.startAnimation(this.poke1Enter);
        }
        else{
          this.setTrainer2Poke(newIndex);
          this.startAnimation(this.poke2Enter);
        }
        this.beginDelay(1200);
      }
        break;
      case 'POKEMON_LEAVE':{
        this.startAnimation(event.data.isPlayer1 ? this.poke1Leave : this.poke2Leave);
        this.beginDelay(1000);
        break;
      }
      case 'POKEMON_FAINT':{
        this.startAnimation(event.data.isPlayer1 ? this.poke1Leave : this.poke2Leave);
        this.beginDelay(1000);
        break;
      }
      case 'APPLY_STATUS':{
        let health: ActiveHealthBar = event.data.appliedToPlayer1 ? this.poke1HealthBar : this.poke2HealthBar;
        health.setStatus(event.data.status, this.statusTextures);
        this.beginDelay(1000);
        break;
      }
      case 'APPLY_VOLATILE':{
        let health: ActiveHealthBar = event.data.appliedToPlayer1 ? this.poke1HealthBar : this.poke2HealthBar;
        health.addVolatile(event.data.effect);
        this.beginDelay(1500);
        break;
      }
      case 'APPLY_FIELD_EFFECT':{
        let fieldEffects: FieldEffectsText = event.data.appliedToPlayer1Side ? this.player1FieldEffects : this.player2FieldEffects;
        fieldEffects.addEffect(event.data.fieldEffect);
        this.beginDelay(1500);
        break;
      }
      case 'REMOVE_VOLATILE':{
        let health: ActiveHealthBar = event.data.removedFromPlayer1 ? this.poke1HealthBar : this.poke2HealthBar;
        health.removeVolatile(event.data.effect);
        this.beginDelay(1500);
        break;
      }
      case 'REMOVE_FIELD_EFFECT':{
        let fieldEffects: FieldEffectsText = event.data.removedFromPlayer1Side ? this.player1FieldEffects : this.player2FieldEffects;
        fieldEffects.removeEffect(event.data.fieldEffect);
        this.beginDelay(1500);
        break;
      }
      case 'WEATHER_CHANGE':
        this.setWeather(event.data.weather);
        this.beginDelay(1200);
        break;
      case 'STAT_CHANGE':
        //TODO
        this.beginDelay(1000);
        break;
      case 'DEBUG_MESSAGE':
        this.beginDelay(1000);
        break;
      default:
        console.error(`Unimplemented event type: ${event.type}`)
    }
  }
}

