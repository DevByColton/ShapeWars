<a href="https://youtu.be/1bVX6a6VZ1I" target="_blank" rel="noopener noreferrer">Current build gameplay video on YouTube</a><br>
<a href="https://cohmoe.itch.io/shape-wars" target="_blank" rel="noopener noreferrer">itch.io link to download</a>

<p class="lato-bold">// MY GOAL FOR SHAPE WARS</p>
<hr class="mb-2">
<p class="mb-1">
  Shapes Wars is a <a href="https://store.steampowered.com/app/8400/Geometry_Wars_Retro_Evolved/" target="_blank" rel="noopener noreferrer">Geometry Wars</a> clone with my own twist. 
  My main purpose for creating this game was to program and design a complete game using C++. I used <a href="https://www.sfml-dev.org/" target="_blank" rel="noopener noreferrer">SFML</a>
  for the window, graphics, and audio systems. I referenced <a href="https://github.com/MonoGame/MonoGame.Samples/blob/3.8.2/NeonShooter/README.md" target="_blank" rel="noopener noreferrer">this tutorial for creating a Geometry Wars clone in C#/Monogame</a>
  for the maths involved in the background warping grid, particles, color generation based off hue, and rotation using quaternions. Systems of my own creation include snake and dodger enemies, nukes, and buff drops from enemies. I also designed the UI
  for handling buffs, lives, bombs, high score, and multiplier while taking up as little room as possible on the screen. This also includes the start menu, pause menu, and options menu.
  I wrote all of these system using some principles from <a href="https://gameprogrammingpatterns.com/" target="_blank" rel="noopener noreferrer">Game Programming Patterns</a>.
  In creating this game I was able to learn a lot about C++, and I was able to flex all aspects of the game creation process by designing the code, the UI, creating my own assets, and 
  designing the gameplay to make it feel challenging, but still beatable, balanced, and fun.
</p>
<p class="lato-bold">// TECHNICAL SYSTEMS IMPLEMENTED</p>
<hr class="mb-2">
<ul class="mb-1">
  <li>Event driven input handling system. Including Mouse and Keyboard, Xbox (One or 360) controller, and Dualsense controller. Input can be from multiple sources at once, should you want to do that</li>
  <li>Fixed timestep for the core gameplay loop (60 fps) using chrono to track delta time</li>
  <li>Implemented object pools to allocate memory up front for enemies, particles, and bullets</li>
  <li>Randomized enemy spawning, with each enemy type having its own behavior logic using lambdas</li>
  <li>Tracking score and gameplay time, reading and writing high scores to a text file on startup and on achieving a new high score using fstream</li>
  <li>Implemented a gaussian blur shader using SFML shader class and GLSL</li>
  <li>Particle system using an object pool that fades the particle by velocity or its duration left</li>
  <li>Background grid system which is laid out by window size. The lines are drawn using SFMLs vertex primitives</li>
  <li>Start menu, in game pause menu, and options menu that allow editing settings that are saved in a binary file and persisted</li>
</ul>
<p class="lato-bold">// GAMEPLAY SYSTEMS IMPLEMENTED</p>
<hr class="mb-2">
<ul class="mb-1">
  <li>Core gameplay loop that is pause-able, and tracking player lives which resets the game if all lives are lost</li>
  <li>Randomized enemy spawns</li>
  <li>4 unique enemy types: Seeker, Wanderer, Dodger, and Snakes</li>
  <li>Nukes that kill all enemies and black holes on the board</li>
  <li>Black Holes which repel bullets but pull the player and particles in</li>
  <li>Randomized buffs which drop from enemies, the player can store them for later activation in the four slots available</li>
  <li>A boss with movement and lasers attack, the boss appears after a certain score is reached</li>
  <li>Endless mode after the boss is defeated</li>
</ul>
<p class="lato-bold">// FINAL SYSTEMS TO IMPLEMENT</p>
<hr class="mb-2">
<ul>
  <li><b>(Currently in progress)</b>Gameplay sounds and music</li>
  <li>Add the finished build to my itch.io page for free download to play</li>
</ul>
