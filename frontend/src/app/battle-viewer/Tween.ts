import { Point } from "pixi.js";

export class Tween{
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