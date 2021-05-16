#include "pacman.hpp"

void Pacman::free() {
	if(up != NULL) {
		SDL_DestroyTexture(up);
		up = NULL;
	}
	if(right != NULL) {
		SDL_DestroyTexture(right);
		right = NULL;
	}
	if(down != NULL) {
		SDL_DestroyTexture(down);
		down = NULL;
	}
	if(left != NULL) {
		SDL_DestroyTexture(left);
		left = NULL;
	}
	if(die != NULL) {
		SDL_DestroyTexture(die);
		die = NULL;
	}
	if(pacPacCollision != NULL) {
		Mix_FreeChunk(pacPacCollision);
		pacPacCollision = NULL;
	}
}

Pacman::Pacman() {
	rowAligned = colAligned = false;
	screenX = screenY = 0;
	velX = velY = 0;
	maze = NULL;
	boundingRect.clear();
	colliderBox.x = 0;
	colliderBox.y = 0;
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (PACMAN_WIDTH - 1) / 2;
	colliderSphere.center.x = BOX_WIDTH / 2;
	colliderSphere.center.y = BOX_HEIGHT / 2;
	parryCircle.radius = colliderSphere.radius + 10;
	parryCircle.center.x = colliderSphere.center.x;
	parryCircle.center.y = colliderSphere.center.y;
	state = STILL_RIGHT;
	frameCount = 0;
	score = 0;
	lives = 3;
	life = 3;

	respawnPoint.x = 0;
	respawnPoint.y = 0;

	parryCount = 0;
	success = true;
	isDead = false;
	isBuffed = false;
	parry = false;
}

Pacman::Pacman(Maze* maze, Window* window, int tp) {
	
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding;
	
	rowAligned = colAligned = true;		
	type = tp;
	if(type == 1){
		screenX = screenY = padding + dotSize;
	}
	if(type == 2){
		screenX = screenY = maze->getBlockScreenCoordinate(maze->dimension-1, maze->dimension-1).x;
	}
	velX = velY = 0;
	this->maze = maze;
	boundingRect = maze->boundaryRectPacman;
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (PACMAN_WIDTH - 1) / 2;
	colliderSphere.center.x = screenX + BOX_WIDTH / 2;
	colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
	parryCircle.radius = colliderSphere.radius + 10;
	parryCircle.center.x = colliderSphere.center.x;
	parryCircle.center.y = colliderSphere.center.y;
	state = STILL_RIGHT;
	frameCount = 0;
	score = 0;
	lives = 3;
	life = 3;

	respawnPoint.x = screenX;
	respawnPoint.y = screenY;

	parryCount = 0;
	success = true;
	isDead = false;
	isBuffed = false;
	parry = false;

	loadTexture(window);
}

void Pacman::loadTexture(Window* window) {
	SDL_Surface* upSurf = IMG_Load("../img/pacman/upMotion.png");
	if(upSurf == NULL) {
		std::cout << "Unable to load Up motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		up = SDL_CreateTextureFromSurface(window->getRenderer(), upSurf);
		if(up == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(upSurf);
	}

	SDL_Surface* rightSurf = IMG_Load("../img/pacman/rightMotion.png");
	if(rightSurf == NULL) {
		std::cout << "Unable to load Right motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		right = SDL_CreateTextureFromSurface(window->getRenderer(), rightSurf);
		if(right == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(rightSurf);
	}

	SDL_Surface* downSurf = IMG_Load("../img/pacman/downMotion.png");
	if(downSurf == NULL) {
		std::cout << "Unable to load Down motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		down = SDL_CreateTextureFromSurface(window->getRenderer(), downSurf);
		if(down == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(downSurf);
	}

	SDL_Surface* leftSurf = IMG_Load("../img/pacman/leftMotion.png");
	if(leftSurf == NULL) {
		std::cout << "Unable to load Left motion sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		left = SDL_CreateTextureFromSurface(window->getRenderer(), leftSurf);
		if(left == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(leftSurf);
	}
	SDL_Surface* dieSurf = IMG_Load("../img/pacman/die.png");
	if(dieSurf == NULL) {
		std::cout << "Unable to load die sprite! SDL_Image Error: " << IMG_GetError() << "\n";
		success = false;
	}
	else {
		die = SDL_CreateTextureFromSurface(window->getRenderer(), dieSurf);
		if(die == NULL) {
			std::cout << "Unable to create texture from sprite! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		SDL_FreeSurface(dieSurf);
	}

	pacPacCollision = Mix_LoadWAV("../music/collision.wav");
	if(pacPacCollision == NULL) {
		std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << "\n"; 
		success = false;
	}

}




void Pacman::handleEvent(SDL_Event &e, const Uint8* keyStates, int online) {
	if(!isDead) {
		if(type == 2 && e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			int num = 0;
			if(online == 0) {
				if(keyStates[SDL_SCANCODE_UP] == 1) num ++;
				if(keyStates[SDL_SCANCODE_RIGHT] == 1) num ++;
				if(keyStates[SDL_SCANCODE_DOWN] == 1) num ++;
				if(keyStates[SDL_SCANCODE_LEFT] == 1) num ++;
			}	
			if(num <= 1) {
				switch(e.key.keysym.sym) {
					case SDLK_UP: velY = -PACMAN_VEL; state = MOVE_UP; frameCount = 0; break;
					case SDLK_DOWN: velY = PACMAN_VEL; state = MOVE_DOWN; frameCount = 0; break;
					case SDLK_RIGHT: velX = PACMAN_VEL; state = MOVE_RIGHT; frameCount = 0; break;
					case SDLK_LEFT: velX = -PACMAN_VEL; state = MOVE_LEFT; frameCount = 0; break;
					default: break;
				}
			}
		}
		else if(type == 2 && e.type == SDL_KEYUP && e.key.repeat == 0) {
			switch(e.key.keysym.sym) {
				case SDLK_UP: if(velY < 0) { velY = 0; state = STILL_UP; } frameCount = 0; break;
				case SDLK_DOWN: if(velY > 0) { velY = 0; state = STILL_DOWN; } frameCount = 0; break;
				case SDLK_RIGHT: if(velX > 0) { velX = 0; state = STILL_RIGHT; } frameCount = 0; break;
				case SDLK_LEFT: if(velX < 0) { velX = 0; state = STILL_LEFT; } frameCount = 0; break;
				default: break;
			}
		}
		else if(type == 1 && e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			int num = 0;
			if(online == 0) {
				if(keyStates[SDL_SCANCODE_W] == 1) num ++;
				if(keyStates[SDL_SCANCODE_D] == 1) num ++;
				if(keyStates[SDL_SCANCODE_S] == 1) num ++;
				if(keyStates[SDL_SCANCODE_A] == 1) num ++;
			}
			if(num <= 1) {
				switch(e.key.keysym.sym) {
					case SDLK_w: velY = -PACMAN_VEL; state = MOVE_UP; frameCount = 0; break;
					case SDLK_s: velY = PACMAN_VEL; state = MOVE_DOWN; frameCount = 0; break;
					case SDLK_d: velX = PACMAN_VEL; state = MOVE_RIGHT; frameCount = 0; break;
					case SDLK_a: velX = -PACMAN_VEL; state = MOVE_LEFT; frameCount = 0; break;
					default: break;
				}
			}
		}
		else if(type == 1 && e.type == SDL_KEYUP && e.key.repeat == 0) {
			switch(e.key.keysym.sym) {
				case SDLK_w: if(velY < 0) { velY = 0; state = STILL_UP; } frameCount = 0; break;
				case SDLK_s: if(velY > 0) { velY = 0; state = STILL_DOWN; } frameCount = 0; break;
				case SDLK_d: if(velX > 0) { velX = 0; state = STILL_RIGHT; } frameCount = 0; break;
				case SDLK_a: if(velX < 0) { velX = 0; state = STILL_LEFT; } frameCount = 0; break;
				default: break;
			}
		}
		
	}
}

void Pacman::move() {
	
	bool collision = false;
	int size = boundingRect.size();
	if(velX != 0 && !isDead) {
		screenX += velX;
		state = velX > 0 ? MOVE_RIGHT : MOVE_LEFT;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		colliderSphere.center.x += velX;
		parryCircle.center.x += velX;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorCircle(&colliderSphere, &boundingRect[i])) {
				screenX -= velX;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				colliderSphere.center.x -= velX;
				parryCircle.center.x -= velX;
				collision = true;
				state = velX > 0 ? STILL_RIGHT : STILL_LEFT;
				velX = 0;
				break;
			}
		}	
	}
	else if(velY != 0 && !isDead) {
		screenY += velY;
		state = velY > 0 ? MOVE_DOWN : MOVE_UP;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		colliderSphere.center.y += velY;
		parryCircle.center.y += velY;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorCircle(&colliderSphere, &boundingRect[i])) {
				screenY -= velY;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				colliderSphere.center.y -= velY;
				parryCircle.center.y -= velY;
				state = velY > 0 ? STILL_DOWN : STILL_UP;
				collision = true;
				velY = 0;
				break;
			}
		}
	}
}

void Pacman::render(Window* window) {
	if(SDL_GetTicks() - parryStart >= 9000) {
		parry = false;
		parryCount = 0;
	}

	SDL_Color color1[] = { {0xFF, 0x00, 0x00, 0x80}, {0x00, 0xFF, 0xFF, 0x80}, {0xFF, 0x00, 0xFF, 0x80} };
	SDL_Color color2[] = { {0xFF, 0xFF, 0xFF, 0x80}, {0x50, 0xF0, 0xF0, 0x80}, {0x36, 0x49, 0xAA, 0x80} };
	if(type == 1){window->renderCircle(&parryCircle, color1[parryCount / 4]);}
	else if(type == 2){window->renderCircle(&parryCircle, color2[parryCount / 4]);}
	if(parry) {
		parryCount ++;
		if(parryCount == 11)
			parryCount = 0;
	}
	SDL_Rect stillPosition = {0, 0, 45, 45};
	SDL_Rect movingPosition = {(frameCount / ANIMATION_FRAMES) * 45, 0, 45, 45};

	if(!isDead) {	
		switch(state) {
			case STILL_UP: 
				window->renderTexture(up, &stillPosition, &colliderBox);
				break;
			case STILL_RIGHT: 
				window->renderTexture(right, &stillPosition, &colliderBox);
				break;
			case STILL_DOWN: 
				window->renderTexture(down, &stillPosition, &colliderBox);
				break;
			case STILL_LEFT: 
				window->renderTexture(left, &stillPosition, &colliderBox);
				break;			
			case MOVE_UP:
				window->renderTexture(up, &movingPosition, &colliderBox);
				frameCount++;
				break;
			case MOVE_RIGHT:
				window->renderTexture(right, &movingPosition, &colliderBox);
				frameCount++;
				break;
			case MOVE_DOWN:
				window->renderTexture(down, &movingPosition, &colliderBox);
				frameCount++;
				break;
			case MOVE_LEFT:
				window->renderTexture(left, &movingPosition, &colliderBox);
				frameCount++;
				break;			
		}
		if(frameCount == 16) {
			frameCount = 0;
		}
	}
	else {
		movingPosition.x = (frameCount / 6) * 45;
		window->renderTexture(die, &movingPosition, &colliderBox);
		frameCount ++;
		if(frameCount == 66) {
			lives --;
			frameCount = 0;
			isDead = false;
			screenX = respawnPoint.x;
			screenY = respawnPoint.x;
			velX = velY = 0;
			colliderBox.x = respawnPoint.x;
			colliderBox.y = respawnPoint.y;
			colliderSphere.center.x = screenX + BOX_WIDTH / 2;
			colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
			parryCircle.center.x = colliderSphere.center.x;
			parryCircle.center.y = colliderSphere.center.y;
			state = STILL_UP;
		}
	}	
}

bool Pacman::collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2) {
	int top1 = rect1->y, top2 = rect2->y, bottom1 = rect1->y + rect1->h, bottom2 = rect2->y + rect2->h,
		right1 = rect1->x + rect1->w, right2 = rect2->x + rect2->w, left1 = rect1->x, left2 = rect2->x;

	if(bottom1 <= top2 || top1 >= bottom2 || left1 >= right2 || left2 >= right1) {
		return false;
	}	
	else {
		return true;
	}
}

bool Pacman::collisionDetectorCircle(Circle* circle, SDL_Rect *rect) {
	SDL_Point closestPoint;
	if(circle->center.x < rect->x) {
		closestPoint.x = rect->x;
	}
	else if(circle->center.x > rect->x + rect->w) {
		closestPoint.x = rect->x + rect->w;
	}
	else {
		closestPoint.x = circle->center.x;
	}
	if(circle->center.y < rect->y) {
		closestPoint.y = rect->y;
	}
	else if(circle->center.y > rect->y + rect->h) {
		closestPoint.y = rect->y + rect->h;
	}
	else {
		closestPoint.y = circle->center.y;
	}
	// check collision
	int distanceSq = (closestPoint.x - circle->center.x) * (closestPoint.x - circle->center.x) + 
				     (closestPoint.y - circle->center.y) * (closestPoint.y - circle->center.y);
	if(distanceSq < circle->radius * circle->radius) {
		return true;
	} 
	else {
		return false;
	}
}


void Pacman::pacpacCollision(Pacman* pac) {
	if(collisionDetectorCirclesOnly(&(this->colliderSphere), &(pac->colliderSphere))) {
		Mix_PlayChannel(17, pacPacCollision, 0);
		int x1 = this->colliderSphere.center.x, x2 = pac->colliderSphere.center.x;
		int y1 = this->colliderSphere.center.y, y2 = pac->colliderSphere.center.y;
		int sepX = x1 >= x2 ? x1 - x2 : x2 - x1;
		int sepY = y1 >= y2 ? y1 - y2 : y2 - y1;
		if(sepX < sepY) {
			if(y1 < y2) {
				this->velY = -7*PACMAN_VEL;
				this->velX = 0;
				pac->velY = 7*PACMAN_VEL;
				pac->velX = 0;
				this->move();
				pac->move();
				this->velY = this->velX = pac->velX = pac->velY = 0; 
				this->state = STILL_UP;
				pac->state = STILL_DOWN;
			}
			else {
				this->velY = 7*PACMAN_VEL;
				this->velX = 0;
				pac->velY = -7*PACMAN_VEL;
				pac->velX = 0; 	
				this->move();
				pac->move();
				this->velY = this->velX = pac->velX = pac->velY = 0; 
				this->state = STILL_DOWN;
				pac->state = STILL_UP;
			}
		}
		else {
			if(x1 < x2) {
				this->velX = -7*PACMAN_VEL;
				this->velY = 0;
				pac->velX = 7*PACMAN_VEL;
				pac->velY = 0; 
				this->move();
				pac->move();
				this->velY = this->velX = pac->velX = pac->velY = 0; 
				this->state = STILL_LEFT;
				pac->state = STILL_RIGHT;
			}
			else {
				this->velX = 7*PACMAN_VEL;
				this->velY = 0;
				pac->velX = -7*PACMAN_VEL;
				pac->velY = 0; 	
				this->move();
				pac->move();
				this->velY = this->velX = pac->velX = pac->velY = 0; 
				this->state = STILL_RIGHT;
				pac->state = STILL_LEFT;
			}
		}
	}
}


bool Pacman::collisionDetectorCirclesOnly(Circle* circle1, Circle* circle2) {
	int r1 = circle1->radius, r2 = circle2->radius;
	SDL_Point point1 = circle1->center, point2 = circle2->center;

	// check collision
	int distanceSq = (point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y);
	if(distanceSq < (r1 + r2) * (r1 + r2)) {
		return true;
	}
	else {
		return false;
	}
}