import { Text, Sprite, Point, Application, Container, ContainerChild, Texture, Assets } from "pixi.js";
import { TEXTBOX_WIDTH } from "./Constants";

const TEXT_COLOR = '#262626';

export class FieldEffectsText{
    private list: Array<string> = new Array<string>();
    public text: Text = new Text({
        text: '',
        style: {
            fontFamily: 'Unageo-Bold',
            fontSize: 25,
            wordWrap: true,
            wordWrapWidth: TEXTBOX_WIDTH,
            align: 'center',
            fill: TEXT_COLOR
        }
    });

    screenSpacePos: Point;

    constructor(screenSpacePos: Point){
        this.screenSpacePos = screenSpacePos;
        this.text.anchor.set(0.5);
    }

    addToStage(stage: Container<ContainerChild>){
        stage.addChild(this.text);
    }

    private updateList(){
        this.text.text = '';
        for (let i = 0; i < this.list.length; i++){
            this.text.text += this.list[i];
            if (i != this.list.length-1){
                this.text.text += ", ";
            }
        }
    }

    addEffect(effect: string){
        this.list.push(effect);
        this.updateList();
    }

    removeEffect(effect: string){
        this.list = this.list.filter(eff => eff != effect);
        this.updateList();
    }
}