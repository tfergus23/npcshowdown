import { Tween } from "./Tween";
import { Point } from "pixi.js";

export class Animation{
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