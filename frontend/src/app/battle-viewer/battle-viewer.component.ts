import { Component, ElementRef, ViewChild } from '@angular/core';
import { Application, Assets, Sprite } from 'pixi.js';

@Component({
  selector: 'app-battle-viewer',
  templateUrl: './battle-viewer.component.html',
  styleUrls: ['./battle-viewer.component.css']
})
export class BattleViewerComponent {
  @ViewChild('pixiContainer', { static: true }) pixiContainer!: ElementRef;
  ngOnInit(){
    setTimeout((async () =>
      {
          // Create a new application
          const app = new Application();

          
        
          // Initialize the application
          await app.init({ background: '#1099bb', resizeTo: this.pixiContainer.nativeElement });
      
          // Append the application canvas to the document body
          this.pixiContainer.nativeElement.appendChild(app.canvas);
      
          // Load the bunny texture
          const texture = await Assets.load('https://pixijs.com/assets/bunny.png');
      
          // Create a bunny Sprite
          const bunny = new Sprite(texture);
      
          // Center the sprite's anchor point
          bunny.anchor.set(0.5);
      
          // Move the sprite to the center of the screen
      
          app.stage.addChild(bunny);
      
          // Listen for animate update
          app.ticker.add((time) =>
          {
              // Just for fun, let's rotate mr rabbit a little.
              // * Delta is 1 if running at 100% performance *
              // * Creates frame-independent transformation *
              bunny.x = app.screen.width / 2;
              bunny.y = app.screen.height / 2;
              bunny.rotation += 0.1 * time.deltaTime;
              app.resize();
          });
      }));
  }
}