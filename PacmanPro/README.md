# PACMAN PRO 
Subtask 1 of Task 2 of course COP290, Spring 2021  

--------------------------------
PACMAN PRO: SYSTEM REQUIREMENTS  
--------------------------------  
Recommended OS: Ubuntu 20.04 LTS  
Recommended Resolution: 1920x1080 or higher  
Recommended Networking: LAN on different devices  
(two terminals running on the same device can slow down the rendering, not recommended)  

NOTE: Game's performance not guaranteed on other platforms  

------------------------------------------------------------
PACMAN PRO: INSTALLATION INSTRUCTIONS (on Ubuntu 20.04 LTS)  
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
	
(you may have to run *sudo apt-get update* before running these commands, or if there is an error)  

-----------------------------
PACMAN PRO: RUNNING THE GAME  
-----------------------------
- Clone the repo https://github.com/ArnavT005/Maze_Game  
- Change to *PacmanPro/src* directory  
- Open terminal  
- LOCAL:  
  - run *make* or *make game*  
  - execute game by running the command *./game*  
- ONLINE:  
  - run *make* to compile both server-client executables together  
  - run *make server* (to only compile server executable)
  - run *make game* (to only compile client executable)
  - run server on a machine using *./server*  
  - run game (client) on two different machines/terminals using *./game*  
  (Note that the server disconnects when either of the clients disconnect (closes game).  
   The servers needs to be restarted in case the client disconnects, for reconnection.)  
- ENJOY!  

---------------------
PACMAN PRO: FEATURES
---------------------
We have added some really awesome *pro* features for you :  

- GENERAL:  
	- Add 2 player game modes (Local/Online)  
	- Add networking. Players can now play together even when on different machines. Only LAN supported as of now.
	- Add *safe* areas for players (GHOSTS DENIED!)  
	- Background theme is chosen at random from 5 predefined backgrounds.  
	  This can act as a breath of fresh air, when playing multiple times.  
	- New maze is generated every time the game is started.  
	  (Note that it is ensured that there are no closed areas)  
	- *Rematch* option is now available at the end of the match.  
	  ( Now you can play for hours ;) )  
	- Players can enjoy all the game modes (local/online) as long as they are connected to the server.  
	- A message is printed on terminal when the connection between client and server is slow. Slow connection may lead to lag and/or loss of information.
	- New game mode: DARK MODE (scary!).  
	  (Players have limited visibility and are haunted by ghosts, a real challenge!)  
	- Various new sound effects have been added for different in-game activities  
- PLAYER:  
	- Players can now teleport using any of the 4 portals present.  
	  Destination portal is picked up at random (surprise!)  
	- Added a new *zone* around the players, a.k.a Parry Zone.  
	  Players can now parry ghosts even when they are not scared.  
	  (Can be useful when you are at a dead end. It has a cooldown though!)  
	- Players can now collide with one another, which can lead to unpleasant encounters!  
	  (It is a fight to survive and win!)  
- GHOST:  
	- Ghosts can now split up at a specific time leading to more number of them.  
	- Add new mode: DARK MODE  
	- Improved Ghost AI. Inky now speeds up when going crazy! Be wary of him.  

Disclaimer: Our two player pacman (local/online) builds up on the original single player pacman. Many of the original pacman mechanics have been recreated, and hence are omitted from this section. 

-----------------------------------------
PACMAN PRO: RESOURCES USED IN DEVELOPMENT   
-----------------------------------------
SDL, SDL_image, SDL_mixer, SDL_ttf:  
https://lazyfoo.net/tutorials/SDL/index.php  

SDL_net:  
http://www.sdltutorials.com/sdl-net-part-1-user-tutorial  
http://www.sdltutorials.com/sdl-net-part-2-user-tutorial  
https://www.libsdl.org/projects/SDL_net/docs/SDL_net.pdf  

IMAGES and FONTS:  
Present in *img* and *font* folder. All open source.  

MUSIC:  
Background music-  
Music: [Space Ambience](https://serpentsoundstudios.com) by Alexander Nakarada 
Licensed under Creative Commons BY Attribution 4.0 [License](https://creativecommons.org/licenses/by/4.0/)  
All other sound effects in *music* folder are open source.



