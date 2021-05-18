#include "ghost.hpp"

void Ghost::free() {
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
}

Ghost::Ghost() {
	
	rowAligned = colAligned = true;
	maze = NULL;
	boundingRect.clear();
    screenX = 0;
    screenY = 0;
	velX = velY = 0;
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (GHOST_WIDTH - 1) / 2;
	colliderSphere.center.x = screenX + BOX_WIDTH / 2;
	colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
	direction = INITIAL;
	state = STILL_UP;
	success = true;
	frameCount = 0;
	dest.clear();
	large.clear();
	small.clear();
	destinationX = 0;
	destinationY = 0;
	reached = false;
	distance = 0;
	targetAcquired = false;
	reachedStart = 0;
	radarRadius = 0;
	end = false;

}

Ghost::Ghost(Maze* maze, Window* window, int startX, int startY) {

	rowAligned = colAligned = true;
	this->maze = maze;
	boundingRect = maze->boundaryRectGhost;
    screenX = maze->getBlockScreenCoordinate(startX, startY).x;
    screenY = maze->getBlockScreenCoordinate(startX, startY).y;
	velX = velY = 0;
	colliderBox.x = screenX;
	colliderBox.y = screenY;
	colliderBox.w = BOX_WIDTH;
	colliderBox.h = BOX_HEIGHT;
	colliderSphere.radius = (GHOST_WIDTH - 1) / 2;
	colliderSphere.center.x = screenX + BOX_WIDTH / 2;
	colliderSphere.center.y = screenY + BOX_HEIGHT / 2;
	direction = INITIAL;
	state = STILL_UP;
	success = true;
	frameCount = 0;
	dest.clear();
	large.clear();
	small.clear();
	destinationX = startX;
	destinationY = startY;
	reached = false;
	distance = 0;
	targetAcquired = false;
	reachedStart = 0;
	radarRadius = 0;
	end = false;
	loadTexture(window);
}


void Ghost::loadTexture(Window* window) {
	std::string path;
	path = "Pinky";

	SDL_Surface* upSurf = IMG_Load(("../img/" + path + "/upMotion.png").c_str());
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
	SDL_Surface* rightSurf = IMG_Load(("../img/" + path + "/rightMotion.png").c_str());
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

	SDL_Surface* downSurf = IMG_Load(("../img/" + path + "/downMotion.png").c_str());
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

	SDL_Surface* leftSurf = IMG_Load(("../img/" + path + "/leftMotion.png").c_str());
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
}

void Ghost::checkAlignment() {
	
	int blockSize = maze->blockSize,
		dotSize = maze->dotSize,
		padding = maze->padding,
		offset = dotSize + blockSize,
		residue = (padding - blockSize + 100 * offset) % offset; // positive quantity
	if(screenX % offset == residue) {
		colAligned = true;
	}
	else {
		colAligned = false;
	}
	if(screenY % offset == residue) {
		rowAligned = true;
	}
	else {
		rowAligned = false;
	}
}

void Ghost::update(Window* window) {
  	bool move = true;
  	if(reached && !targetAcquired)
  		move = false;
  	// else if(targetAcquired)
  	// 	targetAcquired = false;
    checkAlignment();
	if(rowAligned && colAligned){
	  	direction = 0;
		if(move) {
			direction = BFS(destinationX, destinationY);
			if(direction == 0) {
				reached = true;
				targetAcquired = false;
				reachedStart = SDL_GetTicks();
				SDL_Point point = maze->getBlockScreenCoordinate(destinationX, destinationY);
				int size = large.size();
				for(int i = 0; i < size; i ++) {
					if(point.x + 7 == large[i].location.x && point.y + 7 == large[i].location.y) {
						large[i].free();
						large.erase(large.begin() + i);
						break;
					}
				}
				small.clear();
			}
			else {
				reached = false;
				int size = small.size();
				for(int i = 0; i < size; i ++) {
					if(screenX + 15 == small[i].location.x && screenY + 15 == small[i].location.y) {
						small[i].free();
						small.erase(small.begin() + i);
						break;
					}
				}
			}
			if(reached) {
		    	if(dest.size() == 0)
		    		end = true;
		    	std::pair<int, int> loc = chooseLoc(window);
		    	destinationX = loc.first;
		    	destinationY = loc.second;
		    }
		}
	}
	switch(direction) {
		case UP: velX = 0; velY = -GHOST_VEL; state = MOVE_UP; break;
		case DOWN: velX = 0; velY = GHOST_VEL; state = MOVE_DOWN; break;
		case RIGHT: velY = 0; velX = GHOST_VEL; state = MOVE_RIGHT; break;
		case LEFT: velY = 0; velX = -GHOST_VEL; state = MOVE_LEFT; break;
		default: 
			if(velX > 0)
				state = STILL_LEFT;	
			else if(velX < 0)
				state = STILL_RIGHT;
			else if(velY > 0)
				state = STILL_DOWN;
			else 
				state = STILL_UP;
			velY = 0;
			velX = 0;
			break;
	}
}

void Ghost::move() {
	
	bool collision = false;
	int size = boundingRect.size();
	if(velX != 0) {
		screenX += velX;
		state = velX > 0 ? MOVE_RIGHT : MOVE_LEFT;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		colliderSphere.center.x += velX;		
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
				screenX -= velX;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				colliderSphere.center.x -= velX;
				collision = true;
				state = velX > 0 ? STILL_RIGHT : STILL_LEFT;
				break;
			}
		}	
	}
	else if(velY != 0) {
		screenY += velY;
		state = velY > 0 ? MOVE_DOWN : MOVE_UP;
		colliderBox.x = screenX;
		colliderBox.y = screenY;
		colliderSphere.center.y += velY;
		for(int i = 0; i < size; i ++) {
			if(collisionDetectorRect(&colliderBox, &boundingRect[i])) {
				screenY -= velY;
				colliderBox.x = screenX;
				colliderBox.y = screenY;
				colliderSphere.center.y -= velY;
				state = velY > 0 ? STILL_DOWN : STILL_UP;
				collision = true;
				break;
			}
		}
	}
}

void Ghost::render(Window* window) {
	SDL_Color color = {0x00, 0x00, 0xFF, 0xFF};
	SDL_Rect stillPosition = {0, 0, 39, 45};
	SDL_Rect movingPosition = {(frameCount / (2 * ANIMATION_FRAMES)) * 39, 0, 39, 45};
	SDL_Rect onScreenRect = {colliderBox.x + 3, colliderBox.y, 39, 45};

	switch(state) {	
		case STILL_UP: 
			window->renderTexture(up, &stillPosition, &onScreenRect);
			break;
		case STILL_RIGHT: 
			window->renderTexture(right, &stillPosition, &onScreenRect);
			break;
		case STILL_DOWN: 
			window->renderTexture(down, &stillPosition, &onScreenRect);
			break;
		case STILL_LEFT: 
			window->renderTexture(left, &stillPosition, &onScreenRect);
			break;		
		case MOVE_UP:
			window->renderTexture(up, &movingPosition, &onScreenRect);			
			frameCount ++;
			break;
		case MOVE_RIGHT:
			window->renderTexture(right, &movingPosition, &onScreenRect);			
			frameCount ++;
			break;
		case MOVE_DOWN:
			window->renderTexture(down, &movingPosition, &onScreenRect);			
			frameCount ++;
			break;
		case MOVE_LEFT:
			window->renderTexture(left, &movingPosition, &onScreenRect);			
			frameCount ++;
			break;			
	}
	if(reached && !targetAcquired && !end) {
		renderRadar(window);
		
		radarRadius += radarRadius / 45 + 1 > 5 ? 5: radarRadius / 45 + 1;
	}
	if(frameCount == 8) {
		frameCount = 0;
	}
}

int Ghost::BFS(int destX, int destY){
    if(small.size() == 0 || small.size() == 1)
    	return 0;
    else {
    	Eatable item = small[small.size() - 2];
    	int x = item.location.x - 15;
    	int y = item.location.y - 15;
    	if(x > screenX)
    		return RIGHT;
    	else if(x < screenX)
    		return LEFT;
    	else if(y > screenY)
    		return DOWN;
    	else
    		return UP;
    }
}

bool Ghost::collisionDetectorRect(SDL_Rect* rect1, SDL_Rect* rect2) {
	int top1 = rect1->y, top2 = rect2->y, bottom1 = rect1->y + rect1->h, bottom2 = rect2->y + rect2->h,
		right1 = rect1->x + rect1->w, right2 = rect2->x + rect2->w, left1 = rect1->x, left2 = rect2->x;

	if(bottom1 <= top2 || top1 >= bottom2 || left1 >= right2 || left2 >= right1) {
		return false;
	}	
	else {
		return true;
	}
}

bool Ghost::collisionDetectorCircle(Circle* circle1, Circle* circle2) {
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

bool Ghost::isIn(std::pair<int, int> location) {
	int size = dest.size();
	if(size != 1)
		size -= 1;
	for(int i = 0; i < size; i ++) {
		if(location.first == dest[i].first && location.second == dest[i].second)
			return true;
	}
	return false;
}

std::pair<int, int> Ghost::chooseLoc(Window* window) {
	int size = maze->dimension;
	int len = dest.size();
    std::vector<std::vector<int>> m(size, std::vector<int>(size, -1));
    std::vector<std::vector<std::pair<int, int>>> parent(size, std::vector<std::pair<int, int>>(size));
    for(int i = 0; i < size; i ++) {
    	for(int j = 0; j < size; j ++) {
    		parent[i][j].first = 0;
    		parent[i][j].second = 0;
    	}
    }
    SDL_Point point = maze->screenToBlockCoordinate(screenX, screenY);
    m[point.x][point.y] = 0;
    parent[point.x][point.y] = std::make_pair(-1, -1);
    vector<vector<int>> que;
    que.push_back(vector<int> {point.x, point.y});
    while(!que.empty()){
        vector<int> pt = que[0];
        que.erase(que.begin());
        if(maze->maze[pt[0]][pt[1]].up != ALL_DENIED && maze->maze[pt[0]][pt[1]].up != GHOST_DENIED && pt[0] > 0 && m[pt[0]-1][pt[1]] == -1){
            m[pt[0]-1][pt[1]] = m[pt[0]][pt[1]] + 1;
            parent[pt[0]-1][pt[1]] = std::make_pair(pt[0], pt[1]);
            que.push_back(vector<int> {pt[0]-1, pt[1]});
            if(isIn(std::make_pair(pt[0]-1, pt[1]))) {
            	distance += m[pt[0]-1][pt[1]];
            	for(int i = 0; i < len; i ++) {
            		if(dest[i].first == pt[0]-1 && dest[i].second == pt[1]) {
            			dest.erase(dest.begin() + i);
            			break;
            		}
            	}
            	int x = pt[0] - 1;
            	int y = pt[1];
            	Eatable item;
            	if(x != -1 && y != -1) {
            		int xP = parent[x][y].first;
            		int yP = parent[x][y].second;
            		while(x != -1 && y != -1) {
            			SDL_Point point = maze->getBlockScreenCoordinate(x, y);
            			item.setValues(point.x + 15, point.y + 15, 15, 15, TEN_POINT, window);
            			small.push_back(item);
            			xP = parent[x][y].first;
            			yP = parent[x][y].second;
            			x = xP;
            			y = yP;
         
            		}	
            	}
            	return std::make_pair(pt[0]-1, pt[1]);
            }
        }
        if(maze->maze[pt[0]][pt[1]].down != ALL_DENIED && maze->maze[pt[0]][pt[1]].down != GHOST_DENIED && pt[0] < size - 1 && m[pt[0]+1][pt[1]] == -1){
            m[pt[0]+1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0]+1, pt[1]});
            parent[pt[0]+1][pt[1]] = std::make_pair(pt[0], pt[1]);
            if(isIn(std::make_pair(pt[0]+1, pt[1]))) {
            	distance += m[pt[0]+1][pt[1]];
            	for(int i = 0; i < len; i ++) {
            		if(dest[i].first == pt[0]+1 && dest[i].second == pt[1]) {
            			dest.erase(dest.begin() + i);
            			break;
            		}
            	}
            	int x = pt[0] + 1;
            	int y = pt[1];
            	Eatable item;
            	if(x != -1 && y != -1) {
            		int xP = parent[x][y].first;
            		int yP = parent[x][y].second;
            		while(x != -1 && y != -1) {
            			SDL_Point point = maze->getBlockScreenCoordinate(x, y);
            			item.setValues(point.x + 15, point.y + 15, 15, 15, TEN_POINT, window);
            			small.push_back(item);
            			xP = parent[x][y].first;
            			yP = parent[x][y].second;
            			x = xP;
            			y = yP;
         
            		}	
            	}
            	return std::make_pair(pt[0]+1, pt[1]);
            }
        }
        if(maze->maze[pt[0]][pt[1]].right != ALL_DENIED && maze->maze[pt[0]][pt[1]].right != GHOST_DENIED && pt[1] < size - 1 && m[pt[0]][pt[1]+1] == -1){
            m[pt[0]][pt[1]+1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]+1});
            parent[pt[0]][pt[1]+1] = std::make_pair(pt[0], pt[1]);
            if(isIn(std::make_pair(pt[0], pt[1]+1))) {
            	distance += m[pt[0]][pt[1]+1];
            	for(int i = 0; i < len; i ++) {
            		if(dest[i].first == pt[0] && dest[i].second == pt[1]+1) {
            			dest.erase(dest.begin() + i);
            			break;
            		}
            	}
            	int x = pt[0];
            	int y = pt[1] + 1;
            	Eatable item;
            	if(x != -1 && y != -1) {
            		int xP = parent[x][y].first;
            		int yP = parent[x][y].second;
            		while(x != -1 && y != -1) {
            			SDL_Point point = maze->getBlockScreenCoordinate(x, y);
            			item.setValues(point.x + 15, point.y + 15, 15, 15, TEN_POINT, window);
            			small.push_back(item);
            			xP = parent[x][y].first;
            			yP = parent[x][y].second;
            			x = xP;
            			y = yP;
         
            		}	
            	}
            	return std::make_pair(pt[0], pt[1]+1);
            }
        }
        if(maze->maze[pt[0]][pt[1]].left != ALL_DENIED && maze->maze[pt[0]][pt[1]].left != GHOST_DENIED && pt[1] > 0 && m[pt[0]][pt[1]-1] == -1){
            m[pt[0]][pt[1]-1] = m[pt[0]][pt[1]] + 1;
            que.push_back(vector<int> {pt[0], pt[1]-1});
            parent[pt[0]][pt[1]-1] = std::make_pair(pt[0], pt[1]);
            if(isIn(std::make_pair(pt[0], pt[1]-1))) {
            	distance += m[pt[0]][pt[1]-1];
            	for(int i = 0; i < len; i ++) {
            		if(dest[i].first == pt[0] && dest[i].second == pt[1]-1) {
            			dest.erase(dest.begin() + i);
            			break;
            		}
            	}
            	int x = pt[0];
            	int y = pt[1] - 1;
            	Eatable item;
            	if(x != -1 && y != -1) {
            		int xP = parent[x][y].first;
            		int yP = parent[x][y].second;
            		while(x != -1 && y != -1) {
            			SDL_Point point = maze->getBlockScreenCoordinate(x, y);
            			item.setValues(point.x + 15, point.y + 15, 15, 15, TEN_POINT, window);
            			small.push_back(item);
            			xP = parent[x][y].first;
            			yP = parent[x][y].second;
            			x = xP;
            			y = yP;
         
            		}	
            	}
            	return std::make_pair(pt[0], pt[1]-1);
            }
        }
    }
}

void Ghost::renderRadar(Window* window) {
    SDL_SetRenderDrawColor(window->getRenderer(), 0x00, 0x00, 0xFF, 0x50);
    SDL_SetRenderDrawBlendMode(window->getRenderer(), SDL_BLENDMODE_BLEND);
    int n = 0;
    SDL_Point center = colliderSphere.center;
    for(int i = 25; i <= 1000; i ++) {
        for(int j = 25; j <= 1000; j ++) {
            int distanceSq = (center.x - j) * (center.x - j) + (center.y - i) * (center.y - i);
            if(distanceSq <= radarRadius * radarRadius) {
                SDL_Point sample;
                sample.x = j;
                sample.y = i;
                points[n] = sample;  
                n ++;
            }
        }
    }
    SDL_RenderDrawPoints(window->getRenderer(), points, n);
    SDL_SetRenderDrawBlendMode(window->getRenderer(), SDL_BLENDMODE_NONE);
	
	int cood1X = small[0].location.x, cood1Y = small[0].location.y;

	int distanceSq1 = (cood1X - center.x) * (cood1X - center.x) + (cood1Y - center.y) * (cood1Y - center.y);
	int sqRadar = radarRadius * radarRadius;
	if(distanceSq1 <= sqRadar) {
		targetAcquired = true;
		radarRadius = 0;
	}
}
