import { Text, Sprite, Point, Application, Container, ContainerChild, Texture, Assets } from "pixi.js";
import { BattleEventLog, EventLogTrainer } from "./BattleEventLog";
import { TEXTBOX_WIDTH, ACTIVE_HEALTHBAR_WIDTH, ACTIVE_HEALTHBAR_HEIGHT } from "./Constants";
import { Status } from "./Status";

const TEXT_COLOR = '#262626';

export class ActiveHealthBar {
    nameText: Text;
    healthText: Text;
    background: Sprite = new Sprite();
    healthBar: Sprite = new Sprite();
    missingHealthBar: Sprite = new Sprite();
    statusSprite: Sprite = new Sprite();
    volatilesText: Text;
    volatilesList: Array<string> = new Array<string>();

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
                fill: TEXT_COLOR
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
                fontSize: ACTIVE_HEALTHBAR_HEIGHT,
                wordWrap: true,
                wordWrapWidth: TEXTBOX_WIDTH,
                align: 'left',
                fill: TEXT_COLOR
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

        this.statusSprite.visible = false;
        this.statusSprite.scale.set(0.3);
        this.statusSprite.position.x = 70;
        this.statusSprite.position.y = -45;

        this.volatilesText = new Text({
            text: '',
            style: {
                fontFamily: 'Unageo-Bold',
                fontSize: 25,
                wordWrap: true,
                wordWrapWidth: TEXTBOX_WIDTH,
                align: 'left',
                fill: TEXT_COLOR
            }
        });
        this.volatilesText.position.x = -145;
        this.volatilesText.position.y = 15;

        this.container.addChild(this.nameText);
        this.container.addChild(this.healthBar);
        this.container.addChild(this.healthText);
        this.container.addChild(this.background);
        this.container.addChild(this.statusSprite);
        this.container.addChild(this.volatilesText);
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

    setStatus(status: Status, textures: any){
        if (status == 'None'){
            this.statusSprite.visible = false;
        }
        else{
            this.statusSprite.visible = true;
            this.statusSprite.texture = textures[status];
        }
    }

    updateVolatilesText(){
        this.volatilesText.text = '';
        for (let i = 0; i < this.volatilesList.length; i++){
            this.volatilesText.text += this.volatilesList[i];
            if (i != this.volatilesList.length-1){
                this.volatilesText.text += ", ";
            }
        }
    }

    addVolatile(volatile: string){
        this.volatilesList.push(volatile);
        this.updateVolatilesText();
    }

    removeVolatile(volatile: string){
        this.volatilesList = this.volatilesList.filter(vol => vol != volatile);
        this.updateVolatilesText();
    }
}