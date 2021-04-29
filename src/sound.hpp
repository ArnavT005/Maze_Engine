#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>

//Background music
Mix_Music *bground = NULL;
Mix_Music *tenSecTimer = NULL;

//Sound effects
Mix_Chunk *start = NULL;
Mix_Chunk *respawn = NULL;
Mix_Chunk *powerupSmall = NULL;
Mix_Chunk *powerupLarge = NULL;
Mix_Chunk *pacDeath = NULL;
Mix_Chunk *ghostDeath = NULL;
Mix_Chunk *gameOver = NULL;
Mix_Chunk *pacPacCollision = NULL;	

void LoadMusic(){
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());}
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );}
	bground = Mix_LoadMUS( "../music/background.wav" );
	if( bground == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	tenSecTimer = Mix_LoadMUS( "../music/tenSecTimer.wav");
	if( tenSecTimer == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	start = Mix_LoadWAV( "../music/start.wav");
	if( start == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	respawn = Mix_LoadWAV( "../music/respawn.wav");
	if( respawn == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	powerupSmall = Mix_LoadWAV( "../music/powerupSmall.wav");
	if( powerupSmall == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	powerupLarge = Mix_LoadWAV( "../music/powerupLarge.wav");
	if( powerupLarge == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	pacDeath = Mix_LoadWAV( "../music/pacDeath.wav");
	if( pacDeath == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	ghostDeath = Mix_LoadWAV( "../music/ghostDeath.wav");
	if( ghostDeath == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	gameOver = Mix_LoadWAV( "../music/gameOver.wav");
	if( gameOver == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
	pacPacCollision = Mix_LoadWAV( "../music/pacPacCollision.wav");
	if( pacPacCollision == NULL ){printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );}
}

void ClearMusic(){

	Mix_FreeChunk( start );	Mix_FreeChunk( respawn );	Mix_FreeChunk( powerupSmall );	Mix_FreeChunk( powerupLarge );
	Mix_FreeChunk( pacDeath );	Mix_FreeChunk( ghostDeath );	Mix_FreeChunk( gameOver );	Mix_FreeChunk( pacPacCollision );
	
	start = NULL; respawn = NULL; powerupSmall = NULL; powerupLarge = NULL; pacDeath = NULL; ghostDeath = NULL; gameOver = NULL; pacPacCollision = NULL;
	
	Mix_FreeMusic( bground );	Mix_FreeMusic( tenSecTimer );
	tenSecTimer = NULL; 	bground = NULL;
	Mix_Quit();
	
}

