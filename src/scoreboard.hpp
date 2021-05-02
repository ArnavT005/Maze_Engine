#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include "pacman.hpp"



class Scoreboard {
	public:
	void free1();
	Scoreboard();
	Scoreboard(SDL_Rect* rect, Window* window, Pacman* p1, Pacman* p2);
	void free();
	void loadTexture(Window* window);
	void loadRenderedText(Window* window);
	void render(Window* window);

	SDL_Rect location;
	SDL_Texture* bgTexture;
	SDL_Texture* P1;
	SDL_Texture* scoreP1;
	SDL_Texture* livesP1;
	SDL_Texture* P2;
	SDL_Texture* scoreP2;
	SDL_Texture* livesP2;
	SDL_Texture* timer;
	SDL_Texture* pacman;
	SDL_Texture* pacDie;

	TTF_Font*  scoreFont;
	TTF_Font* timerFont;
	std::stringstream timeText;
	std::stringstream scoreP1Text;
	std::stringstream livesP1Text;
	std::stringstream scoreP2Text;
	std::stringstream livesP2Text;

	Pacman* p1; 
	Pacman* p2;

	Uint32 start;
	int time;
	int lifeCount1;
	int lifeCount2;
	int frameCount1;
	int frameCount2;
};

void Scoreboard::free1() {
	if(bgTexture != NULL) {
		SDL_DestroyTexture(bgTexture);
		bgTexture = NULL;
	}
	if(P1 != NULL) {
		SDL_DestroyTexture(P1);
		P1 = NULL;
	}
	if(P2 != NULL) {
		SDL_DestroyTexture(P2);
		P2 = NULL;
	}
	if(scoreFont != NULL) {
		TTF_CloseFont(scoreFont);
		scoreFont = NULL;
	}
	if(timerFont != NULL) {
		TTF_CloseFont(timerFont);
		timerFont = NULL;
	}
	if(pacman != NULL) {
		SDL_DestroyTexture(pacman);
		pacman = NULL;
	}
	if(pacDie != NULL) {
		SDL_DestroyTexture(pacDie);
		pacDie = NULL;
	}
	free();
}


Scoreboard::Scoreboard() {
	location.x = 1025;
	location.y = 0;
	location.w = 380;
	location.h = 1025;
	bgTexture = NULL;
	scoreFont = NULL;
	timerFont = NULL;
	P1 = NULL;
	P2 = NULL;
	scoreP1 = NULL;
	scoreP2 = NULL;
	livesP2 = NULL;
	livesP1 = NULL;
	timer = NULL;
	pacman = NULL;
	pacDie = NULL;
	time = 0;
	lifeCount1 = 3;
	lifeCount2 = 3;
	frameCount1 = 0;
	frameCount2 = 0;
	p1 = NULL;
	p2 = NULL;
}

Scoreboard::Scoreboard(SDL_Rect* rect, Window* window, Pacman* p1, Pacman* p2) {
	location = *rect;
	bgTexture = NULL;
	scoreFont = NULL;
	timerFont = NULL;
	P1 = NULL;
	P2 = NULL;
	scoreP1 = NULL;
	scoreP2 = NULL;
	livesP2 = NULL;
	livesP1 = NULL;
	timer = NULL;
	pacman = NULL;
	pacDie = NULL;
	time = 0;
	timeText.str("");
	scoreP1Text.str("");SDL_Rect life1Rect1 = {1050, 145, 45, 45};
	SDL_Rect life1Rect2 = {1095, 145, 45, 45};
	SDL_Rect life1Rect3 = {1140, 145, 45, 45};
	livesP1Text.str("");
	scoreP2Text.str("");
	livesP2Text.str("");
	lifeCount1 = 3;
	lifeCount2 = 3;
	frameCount1 = 0;
	frameCount2 = 0;
	this->p1 = p1;
	this->p2 = p2;
	loadTexture(window);
} 

void Scoreboard::free() {
	if(scoreP1 != NULL) {
		SDL_DestroyTexture(scoreP1);
		scoreP1 = NULL;
	}
	if(scoreP2 != NULL) {
		SDL_DestroyTexture(scoreP2);
		scoreP2 = NULL;
	}
	if(livesP1 != NULL) {
		SDL_DestroyTexture(livesP1);
		livesP1 = NULL;
	}
	if(livesP2 != NULL) {
		SDL_DestroyTexture(livesP2);
		livesP2 = NULL;
	}
	if(timer != NULL) {
		SDL_DestroyTexture(timer);
		timer = NULL;
	}
}

void Scoreboard::loadTexture(Window* window) {
	SDL_Surface* background = IMG_Load("../img/background2.png");
	if(background == NULL) {
		std::cout << "Unable to load background image! SDL_Image Error: " << IMG_GetError() << "\n";
	}
	else {
		bgTexture = SDL_CreateTextureFromSurface(window->getRenderer(), background);
		if(bgTexture == NULL) {
			std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
		}
		SDL_FreeSurface(background);
	}
	scoreFont = TTF_OpenFont("../font/03314_BaarZeitgeist.ttf", 28);
    if(scoreFont == NULL )
    {
        std::cout << "Unable to load font! SDL_ttf Error: " << TTF_GetError() << "\n"; 
    }
    else
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface* p1 = TTF_RenderText_Solid(scoreFont, "Player 1", textColor);
        if(p1 == NULL)
        {
            std::cout << "Unable to render text texture! SDL_ttf Error: " << TTF_GetError() << "\n";
        }
        else {
        	P1 = SDL_CreateTextureFromSurface(window->getRenderer(), p1);
	        if(P1 == NULL)
	        {
	            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	        }
            SDL_FreeSurface(p1);
    	}
    	SDL_Surface* p2 = TTF_RenderText_Solid(scoreFont, "Player 2", textColor);
        if(p2 == NULL)
        {
            std::cout << "Unable to render text texture! SDL_ttf Error: " << TTF_GetError() << "\n";
        }
        else {
        	P2 = SDL_CreateTextureFromSurface(window->getRenderer(), p2);
	        if(P2 == NULL)
	        {
	            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	        }
            SDL_FreeSurface(p2);
    	}
    }
    SDL_Surface* pacSurf = IMG_Load("../img/pacman/IndividualSprites/right.png");
    if(pacSurf == NULL) {
    	std::cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << "\n";
    }
    else {
    	pacman = SDL_CreateTextureFromSurface(window->getRenderer(), pacSurf);
    	if(pacman == NULL) {
    		std::cout << "Unable to create texture from image! SDL_image Error: " << IMG_GetError() << "\n";
    	}
    	SDL_FreeSurface(pacSurf);
    }
    SDL_Surface* pacDieSurf = IMG_Load("../img/pacman/die.png");
    if(pacDieSurf == NULL) {
    	std::cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << "\n";
    }
    else {
    	pacDie = SDL_CreateTextureFromSurface(window->getRenderer(), pacDieSurf);
    	if(pacDie == NULL) {
    		std::cout << "Unable to create texture from image! SDL_image Error: " << IMG_GetError() << "\n";
    	}
    	SDL_FreeSurface(pacDieSurf);
    }
}

void Scoreboard::loadRenderedText(Window* window) {
    free();
    if(scoreFont == NULL )
    {
        std::cout << "Unable to load font! SDL_ttf Error: " << TTF_GetError() << "\n"; 
    }
    else
    {
        SDL_Color textColor = {0, 0, 0};
        SDL_Surface* scorep1 = TTF_RenderText_Solid(scoreFont, scoreP1Text.str().c_str(), textColor);
        if(scorep1 == NULL)
        {
            std::cout << "Unable to render text texture! SDL_ttf Error: " << TTF_GetError() << "\n";
        }
        else {
        	scoreP1 = SDL_CreateTextureFromSurface(window->getRenderer(), scorep1);
	        if(scoreP1 == NULL)
	        {
	            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	        }
            SDL_FreeSurface(scorep1);
    	}
    	SDL_Surface* lifep1 = TTF_RenderText_Solid(scoreFont, livesP1Text.str().c_str(), textColor);
        if(lifep1 == NULL)
        {
            std::cout << "Unable to render text texture! SDL_ttf Error: " << TTF_GetError() << "\n";
        }
        else {
        	livesP1 = SDL_CreateTextureFromSurface(window->getRenderer(), lifep1);
	        if(livesP1 == NULL)
	        {
	            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	        }
            SDL_FreeSurface(lifep1);
    	}
    	SDL_Surface* scorep2 = TTF_RenderText_Solid(scoreFont, scoreP2Text.str().c_str(), textColor);
        if(scorep2 == NULL)
        {
            std::cout << "Unable to render text texture! SDL_ttf Error: " << TTF_GetError() << "\n";
        }
        else {
        	scoreP2 = SDL_CreateTextureFromSurface(window->getRenderer(), scorep2);
	        if(scoreP2 == NULL)
	        {
	            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	        }
            SDL_FreeSurface(scorep2);
    	}
    	SDL_Surface* lifep2 = TTF_RenderText_Solid(scoreFont, livesP2Text.str().c_str(), textColor);
        if(lifep2 == NULL)
        {
            std::cout << "Unable to render text texture! SDL_ttf Error: " << TTF_GetError() << "\n";
        }
        else {
        	livesP2 = SDL_CreateTextureFromSurface(window->getRenderer(), lifep2);
	        if(livesP2 == NULL)
	        {
	            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	        }
            SDL_FreeSurface(lifep2);
    	}
    	if(SDL_GetTicks() - start < 4000 || SDL_GetTicks() - start < 104000 && SDL_GetTicks() - start > 94000) {
    		textColor.r = 0xFF;
    		textColor.g = 0x00;
    		textColor.b = 0x00;
    		textColor.a = 0xFF;
    	}
    	SDL_Surface* timeSurf = TTF_RenderText_Solid(scoreFont, timeText.str().c_str(), textColor);
        if(timeSurf == NULL)
        {
            std::cout << "Unable to render text texture! SDL_ttf Error: " << TTF_GetError() << "\n";
        }
        else {
        	timer = SDL_CreateTextureFromSurface(window->getRenderer(), timeSurf);
	        if(timer == NULL)
	        {
	            std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
	        }
            SDL_FreeSurface(timeSurf);
    	}
    }
}

void Scoreboard::render(Window* window) {
	window->renderTexture(bgTexture, &location, &location);
	SDL_Rect rectP1 = {1040, 25, 180, 60};
	SDL_Rect rectP2 = {1040, 820, 200, 60};
	SDL_Rect rectScoreP1 = {1040, 85, 200, 60};
	SDL_Rect rectScoreP2 = {1040, 880, 200, 60};
	SDL_Rect rectLivesP1 = {1040, 145, 100, 60};
	SDL_Rect rectLivesP2 = {1040, 930, 100, 60};
	SDL_Rect rectTimer = {1040, 450, 200, 60};

	SDL_Rect life1Rect1 = {1150, 158, 45, 45};
	SDL_Rect life1Rect2 = {1195, 158, 45, 45};
	SDL_Rect life1Rect3 = {1240, 158, 45, 45};

	SDL_Rect life2Rect1 = {1150, 943, 45, 45};
	SDL_Rect life2Rect2 = {1195, 943, 45, 45};
	SDL_Rect life2Rect3 = {1240, 943, 45, 45};

	SDL_Rect srcRect1 = {(frameCount1) / 6 * 45, 0, 45, 45};
	SDL_Rect srcRect2 = {(frameCount2) / 6 * 45, 0, 45, 45};

	if(SDL_GetTicks() - start < 3150) {
		rectTimer.x = 1080;
		rectTimer.w = 60;
		rectTimer.h = 80;
	}
	else if(SDL_GetTicks() - start < 4000) {
		rectTimer.x = 1080;
		rectTimer.w = 80;
		rectTimer.h = 80;
	}
	window->renderTexture(P1, NULL, &rectP1);
	window->renderTexture(scoreP1, NULL, &rectScoreP1);
	window->renderTexture(livesP1, NULL, &rectLivesP1);
	window->renderTexture(P2, NULL, &rectP2);
	window->renderTexture(scoreP2, NULL, &rectScoreP2);
	window->renderTexture(livesP2, NULL, &rectLivesP2);
	window->renderTexture(timer, NULL, &rectTimer);

	// render lives
	if(p1->life == lifeCount1) {
		switch(lifeCount1) {
			case 3:		
				window->renderTexture(pacman, NULL, &life1Rect3);
			case 2:
				window->renderTexture(pacman, NULL, &life1Rect2);
			case 1: 
				window->renderTexture(pacman, NULL, &life1Rect1);
		}
		frameCount1 = 0;
	}
	else {
		switch(lifeCount1) {
			case 1:
				window->renderTexture(pacDie, &srcRect1, &life1Rect1);
				break;
			case 2:
				window->renderTexture(pacman, NULL, &life1Rect1);
				window->renderTexture(pacDie, &srcRect1, &life1Rect2);
				break;
			case 3:
				window->renderTexture(pacman, NULL, &life1Rect1);
				window->renderTexture(pacman, NULL, &life1Rect2);
				window->renderTexture(pacDie, &srcRect1, &life1Rect3);
				break;	
		}
		frameCount1 ++;
		if(frameCount1 == 66) {
			frameCount1 = 0;
			lifeCount1 = p1->life;
		}
	}
	if(p2->life == lifeCount2) {
		switch(lifeCount2) {
			case 3:		
				window->renderTexture(pacman, NULL, &life2Rect3);
			case 2:
				window->renderTexture(pacman, NULL, &life2Rect2);
			case 1: 
				window->renderTexture(pacman, NULL, &life2Rect1);
		}
		frameCount2 = 0;
	}
	else {
		switch(lifeCount2) {
			case 1:
				window->renderTexture(pacDie, &srcRect2, &life2Rect1);
				break;
			case 2:
				window->renderTexture(pacman, NULL, &life2Rect1);
				window->renderTexture(pacDie, &srcRect2, &life2Rect2);
				break;
			case 3:
				window->renderTexture(pacman, NULL, &life2Rect1);
				window->renderTexture(pacman, NULL, &life2Rect2);
				window->renderTexture(pacDie, &srcRect2, &life2Rect3);
				break;	
		}
		frameCount2 ++;
		if(frameCount2 == 66) {
			frameCount2 = 0;
			lifeCount2 = p2->life;
		}
	}
}

