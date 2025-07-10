import { Text, Sprite, Point, Application, Container, ContainerChild, Texture } from "pixi.js";
import { BattleEventLog, EventLogTrainer } from "./BattleEventLog";
import { TEXTBOX_WIDTH, ACTIVE_HEALTHBAR_WIDTH, ACTIVE_HEALTHBAR_HEIGHT } from "./Constants";
import { stagger } from "@angular/animations";

export class ActiveHealthBar {
    nameText: Text;
    healthText: Text;
    background: Sprite = new Sprite();
    healthBar: Sprite = new Sprite();
    missingHealthBar: Sprite = new Sprite();

    container: Container<ContainerChild> = new Container<ContainerChild>();

    health: Point;

    constructor(log: BattleEventLog, pokeIndex: number, trainerNum: number, health: Point, backgroundTexture: Texture, healthBarTexture: Texture, missingHealthBarTexture: Texture){
        const trainer: EventLogTrainer = trainerNum == 0 ? log.trainer1 : log.trainer2;
        const pokemon = trainer.team[pokeIndex];
        this.nameText = new Text({
            text: pokemon.species,
            style: {
                fontFamily: 'Unageo-Bold',
                fontSize: 25,
                wordWrap: true,
                wordWrapWidth: TEXTBOX_WIDTH,
                align: 'left',
                fill: 'white'
            }
        });
        this.nameText.position.x = -78;
        this.nameText.position.y = -30;
        this.nameText.anchor.set(0.5);

        this.health = health;

        this.healthText = new Text({
            text: `${this.health.x.toFixed(0).padStart(3, "0")}/${this.health.y.toFixed(0).padStart(3, "0")}`,
                style: {
                fontFamily: 'Unageo-Bold',
                fontSize: 25,
                wordWrap: true,
                wordWrapWidth: TEXTBOX_WIDTH,
                align: 'left',
                fill: 'white'
            }
        });
        this.healthText.position.x = 105;
        this.healthText.position.y = 0;
        this.healthText.anchor.set(0.5);

        this.healthBar.texture = healthBarTexture;
        this.healthBar.position.x = -45;
        this.healthBar.position.y =  0;
        this.healthBar.anchor.set(0.5);
        this.healthBar.width = ACTIVE_HEALTHBAR_WIDTH;
        this.healthBar.height = ACTIVE_HEALTHBAR_HEIGHT;

        this.missingHealthBar.texture = missingHealthBarTexture;
        this.missingHealthBar.anchor.set(0.5);
        this.healthBar.addChild(this.missingHealthBar);

        this.background.texture = backgroundTexture;
        this.background.anchor.set(0.5);
        this.background.zIndex = -1;
        this.background.scale.set(3, 1);

        this.container.addChild(this.nameText);
        this.container.addChild(this.healthBar);
        this.container.addChild(this.healthText);
        this.container.addChild(this.background);
        
    }

    addToStage(stage: Container<ContainerChild>){
        stage.addChild(this.container);
    }

    update(){
        const currentHealth = this.health.x;
        const maxHealth = this.health.y;
        this.missingHealthBar.width = (ACTIVE_HEALTHBAR_WIDTH - (currentHealth / maxHealth * ACTIVE_HEALTHBAR_WIDTH)) / this.healthBar.scale.x;
        this.missingHealthBar.position.x = ((currentHealth / maxHealth * ACTIVE_HEALTHBAR_WIDTH)) / 2 / this.healthBar.scale.x;
        this.healthText.text = `${this.health.x.toFixed(0).padStart(3, "0")}/${this.health.y.toFixed(0).padStart(3, "0")}`;
    }
}