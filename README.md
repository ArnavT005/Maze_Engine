# PACMAN and Maze Simulation  
Task 2 of course COP290, Spring 2021  

--------------------------------
PACMAN GAME: SYSTEM REQUIREMENTS  
--------------------------------  
Recommended OS: Ubuntu 20.04 LTS  
Recommended Resolution: 1920x1080 or higher  
Recommended Networking: LAN on different devices  
(two terminals running on the same device can slow down the rendering, not recommended)  

NOTE: Game's performance not guaranteed on other platforms  

------------------------------------------------------------
PACMAN GAME: INSTALLATION INSTRUCTIONS (on Ubuntu 20.04 LTS)  
------------------------------------------------------------
- INSTALLING SDL:  
	- sudo apt-get install libsdl2-dev  
- INSTALLING SDL_image:  
 	- sudo apt-get install libsdl2-image-dev  
- INSTALLING SDL_ttf:  
 	- sudo apt-get install libsdl2-ttf-dev  
- INSTALLING SDL_mixer:  
 	- sudo apt-get install libsdl2-mixer-dev  
- INSTALLING SDL_net:  
 	- sudo apt-get install libsdl2-net-dev  
	
(you may have to run "sudo apt-get update" before running these commands, or if there is an error)  

-------------------------
PACMAN GAME: RUNNING GAME  
-------------------------
- Clone this repo  
- Change to "src" directory  
- Open terminal  
- LOCAL:  
  - run "make"  
  - execute game by running the command "./game"  
- ONLINE:  
  - run "make server"  
  - run "make"  
  - run server on a machine using "./server"  
  - run game on two different machines/terminals using "./game"  
  (Note that the server disconnects when either of the client disconnects (closes game).  
   The servers needs to be restarted in case the client disconnects, for reconnection.)  
- ENJOY!  

--------------------------
PACMAN GAME: IDEA/FEATURES
--------------------------
Disclaimer: Our two player pacman (local/online) builds up on the original single player pacman.  

We have added new features (which are our original):  
- GENERAL:  
	- Add 2 player game modes (Local/Online)  
	- Add "safe" areas for players (GHOSTS DENIED!)  
	- Background theme is chosen at random from 5 predefined backgrounds.  
	  This can act as a breath of fresh air, when playing multiple times.  
	- New maze is generated (randomly) every time the game is started.  
	  (Note that it is ensured that there are no closed areas)  
	- "Rematch" option is now available at the end of the match.  
	  (for both Local/Online play. Enjoy!)  
	- New game mode: DARK MODE (scary!).  
	  (Players have limited visibility and are targeted, a real challenge!)  
	- Various new sound effects have been added for different in-game activities  
- PLAYER:  
	- Player can now teleport using any of the 4 portals present.  
	  Destination portal is picked up at random (surprise!)  
	- Added a new "zone" around the players, a.k.a Parry Zone.  
	  Players can now parry ghosts even when they are not scared.  
	  (Can be useful when you are at a dead end. It has a cooldown though!)  
	- Players can now collide with one another, which can lead to unpleasant encounter!  
	  (It is a fight to survive and win!)  
- GHOST:  
	- Ghosts can now split up at a specific time leading to more number of them.  
	- Add new mode: DARK MODE  
	- Modified Ghost AI. Inky now speeds up when going crazy! Be wary of him.  

(Mostly all original pacman mechanics have been recreated, and hence are omitted from this section)  

---------------------------
PACMAN GAME: RESOURCES USED  
---------------------------
SDL, SDL_image, SDL_mixer, SDL_ttf:  
https://lazyfoo.net/tutorials/SDL/index.php  

SDL_net:  
http://www.sdltutorials.com/sdl-net-part-1-user-tutorial  
http://www.sdltutorials.com/sdl-net-part-2-user-tutorial  
https://www.libsdl.org/projects/SDL_net/docs/SDL_net.pdf  

IMAGES and FONTS:  
Present in "img" and "font" folder. All open source.  

MUSIC:  
Background music-  
Music: [Space Ambience] by Alexander Nakarada (www.serpentsoundstudios.com)  
Licensed under Creative Commons BY Attribution 4.0 License  
https://creativecommons.org/licenses/by/4.0/  
Other sound effects/music-  
Present in "music" folder. All open source.  



