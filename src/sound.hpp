#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>

//Background music
Mix_Music *bground = NULL;
Mix_Chunk *tenSecTimer = NULL;

//Sound effects
Mix_Chunk *start = NULL;
Mix_Chunk *retreat = NULL;
Mix_Chunk *siren = NULL;
Mix_Chunk *powerupSmall = NULL;
Mix_Chunk *powerupLarge = NULL;
Mix_Chunk *parry = NULL;
Mix_Chunk *pacDeath = NULL;
Mix_Chunk *ghostDeath = NULL;
Mix_Chunk *gameOver = NULL;
Mix_Chunk *teleport = NULL;
Mix_Chunk *pacPacCollision = NULL;	

void LoadMusic(){
	Mix_AllocateChannels(32);
	tenSecTimer = Mix_LoadWAV( "../music/countdown.mp3");
	if( tenSecTimer == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	bground = Mix_LoadMUS("../music/background.mp3");
	if(bground == NULL) std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << "\n";
	start = Mix_LoadWAV( "../music/intro.wav");
	if( start == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	retreat = Mix_LoadWAV( "../music/ghostRetreat.wav");
	if( retreat == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	powerupSmall = Mix_LoadWAV( "../music/eat10.wav");
	if( powerupSmall == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	powerupLarge = Mix_LoadWAV( "../music/eat50.wav");
	if( powerupLarge == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	pacDeath = Mix_LoadWAV( "../music/pacDeath.wav");
	if( pacDeath == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	ghostDeath = Mix_LoadWAV( "../music/eatGhost.wav");
	if( ghostDeath == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	parry = Mix_LoadWAV("../music/power_pellet.wav");
	if(parry == NULL) std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << "\n"; 
	teleport = Mix_LoadWAV("../music/TeleportUse.mp3");
	if(teleport == NULL) std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << "\n"; 
	pacPacCollision = Mix_LoadWAV("../music/collision.wav");
	if(parry == NULL) std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << "\n"; 
	// gameOver = Mix_LoadWAV( "../music/gameOver.wav");
	// if( gameOver == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	siren = Mix_LoadWAV( "../music/pacMove.wav");
	if(siren == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
}

void ClearMusic(){

	Mix_FreeChunk(start);	
	Mix_FreeChunk(retreat);	
	Mix_FreeChunk(powerupSmall);
	Mix_FreeChunk(powerupLarge);
	Mix_FreeChunk(pacDeath);
	Mix_FreeChunk(ghostDeath);
	Mix_FreeChunk(parry);
	Mix_FreeChunk(pacPacCollision);
	// Mix_FreeChunk(gameOver);
	Mix_FreeChunk(siren);
	Mix_FreeChunk(teleport);

	start = NULL; retreat = NULL; powerupSmall = NULL; powerupLarge = NULL; pacDeath = NULL; ghostDeath = NULL; gameOver = NULL; pacPacCollision = NULL;
	parry = NULL;
	Mix_FreeMusic(bground);
	Mix_FreeChunk(tenSecTimer);
	tenSecTimer = NULL; 	
	bground = NULL;
	
}

