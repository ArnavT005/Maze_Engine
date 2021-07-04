# SIMULATION & ANALYSIS: The Plight of Pinky

---
SYSTEM REQUIREMENTS  
---
Recommended OS: Ubuntu 20.04 LTS  
Recommended Resolution: 1920x1080 or higher  

> NOTE: Simulation's performance not guaranteed on other platforms.  

---
INSTALLATION INSTRUCTIONS (on Ubuntu 20.04 LTS)  
---
- INSTALLING SDL: `sudo apt-get install libsdl2-dev`
- INSTALLING SDL_image: `sudo apt-get install libsdl2-image-dev`   
- INSTALLING SDL_mixer: `sudo apt-get install libsdl2-mixer-dev`  
	
(you may have to run `sudo apt-get update` before running these commands, or if there is an error)  

---
RUNNING THE SIMULATION 
---
- Change to *src* directory.  
- Open terminal.  
- run `make` (this creates both simulator/analysis executable).
- run `make simulator` (this only creates simulator executable). 
- execute simulation by running the command `./simulator` (slow version) or `./simulator 1` (fast version).

---
SIMULATION FEATURES
---
We have added some additional features for you:  

- GENERAL:  
	- Vary Pinky's movement speed by passing an optional command line argument. "1" makes Pinky faster, slow otherwise.
	- Pause/Resume facility added. You can now pause/resume the simulation by pressing key "P". Don't miss out on anything!

---
RUNNING THE ANALYSIS 
---
- Change to *src* directory.  
- Open terminal.  
- run `make` (this creates both simulator/analysis executable).
- run `make analysis` (this only creates analysis executable). 
- execute analysis by running the command `./analysis` (10 iterations) or `./analysis N` (*N* iterations, *N* is any positive integer).

---
ANALYSIS FEATURES
---
We have added some additional features for you:  

- GENERAL:  
	- You can adjust the number of iterations for which the analysis should be done by passing an optional command line argument N. *N* is any positive integer. Default number of iterations is 10.
	- Optimum and Experimental distance values are available in *Analysis.csv* file. This can be used for further analysis/plotting etc.

	  
---
RESOURCES USED IN DEVELOPMENT   
---
SDL, SDL_image, SDL_mixer: [LazyFoo](https://lazyfoo.net/tutorials/SDL/index.php)   

IMAGES and MUSIC:  
Present in *img* and *music* folder. All open source.  
