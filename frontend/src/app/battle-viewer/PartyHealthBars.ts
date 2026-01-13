import { Text, Sprite, Point, Application, Container, ContainerChild, Texture } from "pixi.js";
import { ACTIVE_HEALTHBAR_WIDTH, ACTIVE_HEALTHBAR_HEIGHT } from "./Constants";
import { HtmlParser } from "@angular/compiler";

class PartyHealthBar {
    container: Container<ContainerChild> = new Container<ContainerChild>();
    healthBar: Sprite = new Sprite();
    missingHealthBar: Sprite = new Sprite();
    pokemon: Sprite = new Sprite();
    health: Point;

    constructor(healthBarTexture: Texture, missingHealthBarTexture: Texture, pokemonTexture: Texture, health: Point, player: number){
        this.healthBar.texture = healthBarTexture;
        this.missingHealthBar.texture = missingHealthBarTexture;
        this.pokemon.texture = pokemonTexture;
        this.health = health;

        this.healthBar.texture = healthBarTexture;
        this.healthBar.position.x = 0;
        this.healthBar.position.y =  0;
        this.healthBar.anchor.set(0.5);
        this.healthBar.width = ACTIVE_HEALTHBAR_WIDTH;
        this.healthBar.height = ACTIVE_HEALTHBAR_HEIGHT;

        this.missingHealthBar.texture = missingHealthBarTexture;
        this.missingHealthBar.anchor.set(0.5);
        this.healthBar.addChild(this.missingHealthBar);

        this.pokemon.position.x = player == 1 ? 130 : -125;
        this.pokemon.position.y = 0;
        this.pokemon.anchor.set(0.5);
        this.pokemon.scale.set(0.6);

        this.container.addChild(this.healthBar);
        this.container.addChild(this.pokemon);
    }

    update(){
        const currentHealth = this.health.x;
        const maxHealth = this.health.y;
        this.missingHealthBar.width = (ACTIVE_HEALTHBAR_WIDTH - (currentHealth / maxHealth * ACTIVE_HEALTHBAR_WIDTH)) / this.healthBar.scale.x;
        this.missingHealthBar.position.x = ((currentHealth / maxHealth * ACTIVE_HEALTHBAR_WIDTH)) / 2 / this.healthBar.scale.x;
        //this.healthText.text = `${this.health.x.toFixed(0).padStart(3, "0")}/${this.health.y.toFixed(0).padStart(3, "0")}`;
    }
}

export class PartyHealthBars {
    container: Container<ContainerChild> = new Container<ContainerChild>();
    bars: Array<PartyHealthBar> = [];
    screenSpacePos: Point = new Point(0,0);

    constructor(healthBarTexture: Texture, missingHealthBarTexture: Texture, healths: Array<Point>, textures: Array<Texture>, screenSpacePos: Point, player: number){
        let ypos = -((healths.length * 50)/2);
        for (let i = 0; i < healths.length; i++){
            let hb = new PartyHealthBar(healthBarTexture, missingHealthBarTexture, textures[i], healths[i], player);
            hb.container.position.y = ypos;
            ypos+= 50;
            this.bars.push(hb);
            this.container.addChild(hb.container);
        }
        this.screenSpacePos = screenSpacePos;
        this.container.pivot.x = this.container.width / 2;
        if (player == 1){
            this.container.pivot.x = -this.container.pivot.x;
        }
    }

    addToStage(stage: Container<ContainerChild>){
        stage.addChild(this.container);
    }

    update(){
        for (let i = 0; i < this.bars.length; i++){
            this.bars[i].update();
        }
    }

    disable(){
        this.bars.forEach((bar) =>{
            bar.healthBar.visible = false;
            bar.missingHealthBar.visible = false;
            bar.pokemon.visible = false;
        });
    }

    enable(){
        this.bars.forEach((bar) =>{
            bar.healthBar.visible = true;
            bar.missingHealthBar.visible = true;
            bar.pokemon.visible = true;
        });
    }
}