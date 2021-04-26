#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include "window.hpp"



class Scoreboard {
	public:
	Scoreboard();
	Scoreboard(SDL_Rect* rect, Window* window);
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
	TTF_Font*  scoreFont;
	TTF_Font* timerFont;
	std::stringstream timeText;
	std::stringstream scoreP1Text;
	std::stringstream livesP1Text;
	std::stringstream scoreP2Text;
	std::stringstream livesP2Text;
	Uint32 start;
	int time;
};


Scoreboard::Scoreboard() {
	location.x = 1025;
	location.y = 0;
	location.w = 380;
	location.h = 1025;
	bgTexture = NULL;
	scoreFont = NULL;
	timerFont = NULL;
	time = 0;
}

Scoreboard::Scoreboard(SDL_Rect* rect, Window* window) {
	location = *rect;
	bgTexture = NULL;
	scoreFont = NULL;
	timerFont = NULL;
	time = 0;
	timeText.str("");
	scoreP1Text.str("");
	livesP1Text.str("");
	scoreP2Text.str("");
	livesP2Text.str("");
	loadTexture(window);
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
        if(p1 == NULL)
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
}

void Scoreboard::loadRenderedText(Window* window) {
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
	SDL_Rect rectP2 = {1040, 930, 180, 60};
	SDL_Rect rectscoreP1 = {1040, 85, 200, 60};
	SDL_Rect rectlivesP1 = {1040, 145, 200, 60};
	SDL_Rect rectscoreP2 = {1040, 880, 200, 60};
	SDL_Rect rectlivesP2 = {1040, 820, 200, 60};
	SDL_Rect recttimer = {1040, 450, 200, 60};
	window->renderTexture(P1, NULL, &rectP1);
	window->renderTexture(scoreP1, NULL, &rectscoreP1);
	window->renderTexture(livesP1, NULL, &rectlivesP1);
	window->renderTexture(P2, NULL, &rectP2);
	window->renderTexture(scoreP2, NULL, &rectscoreP2);
	window->renderTexture(livesP2, NULL, &rectlivesP2);
	window->renderTexture(timer, NULL, &recttimer);
}

