#include "maze.hpp"

void Maze::free() {
    if(texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        pacWallHoriz = NULL;
        ghostWallHoriz = NULL;
        pacWallVert = NULL;
        ghostWallVert = NULL;
    }
}

Maze::Maze() {
    dimension = 16;
    blockSize = 45;
    dotSize = 15;
    padding = 25;
    maze.resize(dimension);
    int i, j;
    for(i = 0; i < dimension; i ++) {
        maze[i].resize(dimension);
    }
    for(i = 0; i < dimension; i ++) {
        for(j = 0; j < dimension; j ++) {
            maze[i][j].degree = 4;
            maze[i][j].up = ACCESS_ALLOWED;
            maze[i][j].right = ACCESS_ALLOWED;
            maze[i][j].down = ACCESS_ALLOWED;
            maze[i][j].left = ACCESS_ALLOWED;
            if(i == 0) {
                maze[i][j].up = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(i == dimension - 1) {
                maze[i][j].down = ALL_DENIED;
                maze[i][j].degree --;
            }
            if(j == 0) {
                maze[i][j].left = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(j == dimension - 1) {
                maze[i][j].right = ALL_DENIED;
                maze[i][j].degree --;
            }
        }
    }
    boundaryRectPacman.clear();
    boundaryRectGhost.clear();
}

Maze::Maze(int dimension, int blockSize, int dotSize, int padding) {
    this->dimension = dimension;
    this->blockSize = blockSize;
    this->dotSize = dotSize;
    this->padding = padding;
    maze.resize(dimension);
    int i, j;
    for(i = 0; i < dimension; i ++) {
        maze[i].resize(dimension);
    }
    for(i = 0; i < dimension; i ++) {
        for(j = 0; j < dimension; j ++) {
            maze[i][j].degree = 4;
            maze[i][j].up = ACCESS_ALLOWED;
            maze[i][j].right = ACCESS_ALLOWED;
            maze[i][j].down = ACCESS_ALLOWED;
            maze[i][j].left = ACCESS_ALLOWED;
            if(i == 0) {
                maze[i][j].up = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(i == dimension - 1) {
                maze[i][j].down = ALL_DENIED;
                maze[i][j].degree --;
            }
            if(j == 0) {
                maze[i][j].left = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(j == dimension - 1) {
                maze[i][j].right = ALL_DENIED;
                maze[i][j].degree --;
            }
        }
    }
    boundaryRectPacman.clear();
    boundaryRectGhost.clear();
}

// 0-indexed
SDL_Point Maze::getBlockScreenCoordinate(int i, int j) {
    SDL_Point point = {-1, -1};
    if(i < dimension && j < dimension && i >= 0 && j >= 0) {
        point.x = padding + (j + 1) * dotSize + j * blockSize;
        point.y = padding + (i + 1) * dotSize + i * blockSize;
        return point;
    }
    else 
        return point;
}

// 1-indexed
SDL_Point Maze::getDotScreenCoordinate(int i, int j) {
    SDL_Point point = {-1, -1};
    if(i <= dimension + 1 && j <= dimension + 1 && i >= 1 && j >= 1) {
        point.x = padding + (j - 1) * (blockSize + dotSize);
        point.y = padding + (i - 1) * (blockSize + dotSize);
        return point;
    }
    else 
        return point;
}

// 1-indexed
SDL_Point Maze::screenToDotCoordinate(int x, int y) {
    SDL_Point point = {(y - padding) / (blockSize + dotSize) + 1, (x - padding) / (blockSize + dotSize) + 1};
    return point;
}


// 0-indexed
SDL_Point Maze::screenToBlockCoordinate(int x, int y) {
    int offset = blockSize + dotSize;
    SDL_Point point = {(y + blockSize - padding) / offset - 1, (x + blockSize - padding) / offset - 1};
    return point;
}

void Maze::updateBlock(int i, int j, int degree, int up, int right, int down, int left) {
    maze[i][j].degree = degree;
    maze[i][j].up = up;
    maze[i][j].right = right;
    maze[i][j].down = down;
    maze[i][j].left = left;
}


void Maze::loadTexture(Window* window) {
    SDL_Surface* surf = IMG_Load(("../img/background/background" + bgType + ".png").c_str());
    if(surf == NULL) {
        std::cout << "Unable to load background image! SDL_Image Error: " << IMG_GetError() << "\n";
    }
    else {
        texture = SDL_CreateTextureFromSurface(window->getRenderer(), surf);
        if(texture == NULL) {
            std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
        }
        SDL_FreeSurface(surf);
    }
    SDL_Surface* pacWallsurf = IMG_Load("../img/wall/pacWallHoriz.png");
    if(pacWallsurf == NULL) {
        std::cout << "Unable to load background image! SDL_Image Error: " << IMG_GetError() << "\n";
    }
    else {
        pacWallHoriz = SDL_CreateTextureFromSurface(window->getRenderer(), pacWallsurf);
        if(pacWallHoriz == NULL) {
            std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
        }
        SDL_FreeSurface(pacWallsurf);
    }
    SDL_Surface* ghostWallsurf = IMG_Load("../img/wall/ghostWallHoriz.png");
    if(ghostWallsurf == NULL) {
        std::cout << "Unable to load background image! SDL_Image Error: " << IMG_GetError() << "\n";
    }
    else {
        ghostWallHoriz = SDL_CreateTextureFromSurface(window->getRenderer(), ghostWallsurf);
        if(ghostWallHoriz == NULL) {
            std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
        }
        SDL_FreeSurface(ghostWallsurf);
    }
    SDL_Surface* ghostWallVertsurf = IMG_Load("../img/wall/ghostWallVert.png");
    if(ghostWallVertsurf == NULL) {
        std::cout << "Unable to load background image! SDL_Image Error: " << IMG_GetError() << "\n";
    }
    else {
        ghostWallVert = SDL_CreateTextureFromSurface(window->getRenderer(), ghostWallVertsurf);
        if(ghostWallHoriz == NULL) {
            std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
        }
        SDL_FreeSurface(ghostWallVertsurf);
    }
    SDL_Surface* pacWallVertsurf = IMG_Load("../img/wall/pacWallVert.png");
    if(pacWallVertsurf == NULL) {
        std::cout << "Unable to load background image! SDL_Image Error: " << IMG_GetError() << "\n";
    }
    else {
        pacWallVert = SDL_CreateTextureFromSurface(window->getRenderer(), pacWallVertsurf);
        if(pacWallVert == NULL) {
            std::cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << "\n";
        }
        SDL_FreeSurface(pacWallVertsurf);
    }
}

bool Maze::BFS(){
    
    int size = dimension;
    bool reachable = true;
    std::vector<std::vector<int>> m(size, std::vector<int>(size, -1));
    m[0][0] = 0;
    std::vector<std::vector<int>> que;
    que.push_back(std::vector<int> {0, 0});
    while(!que.empty()){
        std::vector<int> pt = que[0];
        que.erase(que.begin());
        if(maze[pt[0]][pt[1]].up != ALL_DENIED && pt[0] > 0 && m[pt[0]-1][pt[1]] == -1){
            m[pt[0]-1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0]-1, pt[1]});
        }
        if(maze[pt[0]][pt[1]].down != ALL_DENIED && pt[0] < size - 1 && m[pt[0]+1][pt[1]] == -1){
            m[pt[0]+1][pt[1]] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0]+1, pt[1]});
        }
        if(maze[pt[0]][pt[1]].right != ALL_DENIED && pt[1] < size - 1 && m[pt[0]][pt[1]+1] == -1){
            m[pt[0]][pt[1]+1] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0], pt[1]+1});
        }
        if(maze[pt[0]][pt[1]].left != ALL_DENIED && pt[1] > 0 && m[pt[0]][pt[1]-1] == -1){
            m[pt[0]][pt[1]-1] = m[pt[0]][pt[1]] + 1;
            que.push_back(std::vector<int> {pt[0], pt[1]-1});
        }
    }
    int corn1 = dimension / 2 - 2, corn2 = dimension / 2 + 1;
    for(int i = 0; i < dimension; i ++) {
        for(int j = 0; j < dimension; j ++) {
            if(m[i][j] == -1) {
                if(i == corn1 && j ==  corn1 || i == corn1 && j == corn2 || i == corn2 && j == corn1 || i == corn2 && j == corn2)
                    continue;
                else
                    reachable = false;
            }
        }
    }
    
    return reachable;
}


void Maze::refreshMaze(Window* window) {
    window->clearWindow();
    for(int i = 0; i < dimension; i ++) {
        for(int j = 0; j < dimension; j ++) {
            maze[i][j].degree = 4;
            maze[i][j].up = ACCESS_ALLOWED;
            maze[i][j].right = ACCESS_ALLOWED;
            maze[i][j].down = ACCESS_ALLOWED;
            maze[i][j].left = ACCESS_ALLOWED;
            if(i == 0) {
                maze[i][j].up = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(i == dimension - 1) {
                maze[i][j].down = ALL_DENIED;
                maze[i][j].degree --;
            }
            if(j == 0) {
                maze[i][j].left = ALL_DENIED;
                maze[i][j].degree --;
            }
            else if(j == dimension - 1) {
                maze[i][j].right = ALL_DENIED;
                maze[i][j].degree --;
            }
        }
    }
    boundaryRectGhost.clear();
    boundaryRectPacman.clear();
}

void Maze::createBase(Window *window, int x, int y, SDL_Color boundaryColor) {
    
    
    int windowWidth = 0, windowHeight = 0;

    SDL_GetWindowSize(window->getWindow(), &windowWidth, &windowHeight);

    SDL_Rect horizontalLayer = {x, y, windowHeight, padding};
    SDL_Rect verticalLayer = {x, y, padding, windowHeight};

    window->renderTexture(texture, &horizontalLayer, &horizontalLayer);
    window->renderTexture(texture, &verticalLayer, &verticalLayer);

    boundaryRectGhost.push_back(horizontalLayer);
    boundaryRectPacman.push_back(horizontalLayer);
    boundaryRectGhost.push_back(verticalLayer);
    boundaryRectPacman.push_back(verticalLayer);

    horizontalLayer.y += windowHeight - padding;
    verticalLayer.x += windowHeight - padding;

    window->renderTexture(texture, &horizontalLayer, &horizontalLayer);
    window->renderTexture(texture, &verticalLayer, &verticalLayer);

    boundaryRectGhost.push_back(horizontalLayer);
    boundaryRectPacman.push_back(horizontalLayer);
    boundaryRectGhost.push_back(verticalLayer);
    boundaryRectPacman.push_back(verticalLayer);

    SDL_Point startPoint = getDotScreenCoordinate(1, 1);

    SDL_Rect dot = {startPoint.x, startPoint.y, dotSize, dotSize},
             vEdge = {startPoint.x, startPoint.y + dotSize, dotSize, blockSize},
             hEdge = {startPoint.x + dotSize, startPoint.y, blockSize, dotSize},
             block = {startPoint.x + dotSize, startPoint.y + dotSize, blockSize, blockSize};

    SDL_Color dotInnerColor = {0x00, 0x00, 0x00, 0xFF},   // blue
              dotOuterColor = {0x00, 0x00, 0xFF, 0xFF},   // red
              edgeColor = {0x3B, 0x27, 0xBA, 0xFF},       // dark green
              blockColor = {0x00, 0x00, 0x00, 0xFF};      // green

    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);

    int offset = dotSize + blockSize;

    for(int i = 0; i < dimension; i ++) {
        window->renderTexture(texture, &hEdge, &hEdge);
        boundaryRectGhost.push_back(hEdge);
        boundaryRectPacman.push_back(hEdge);
        dot.x += offset;
        window->renderTexture(texture, &dot, &dot);

        boundaryRectGhost.push_back(dot);
        boundaryRectPacman.push_back(dot);
        hEdge.x += offset;

    }

    for(int i = 0; i < dimension; i ++) {
        window->renderTexture(texture, &vEdge, &vEdge);

        boundaryRectGhost.push_back(vEdge);
        boundaryRectPacman.push_back(vEdge);
        block.x = vEdge.x + dotSize;
        block.y = vEdge.y;
        for(int j = 0; j < dimension; j ++) {
            window->renderRect(&block, blockColor);
            vEdge.x += offset;
            if(j < dimension - 1) {
                window->renderRect(&vEdge, blockColor);
            }
            else {
                window->renderTexture(texture, &vEdge, &vEdge);
 
                boundaryRectGhost.push_back(vEdge);
                boundaryRectPacman.push_back(vEdge);
            }
            block.x += offset;
        }
        dot.x = startPoint.x;
        dot.y += offset;
        window->renderTexture(texture, &dot, &dot);
        boundaryRectGhost.push_back(dot);
        boundaryRectPacman.push_back(dot);
        hEdge.x = dot.x + dot.w;
        hEdge.y = dot.y;
        if(i < dimension - 1) {
            for(int j = 0; j < dimension; j ++) {
                window->renderRect(&hEdge, blockColor);
                dot.x += offset;
                if(j < dimension - 1) {
                    window->renderRect(&dot, dotInnerColor);
                }
                else {
                    window->renderTexture(texture, &dot, &dot);
                    boundaryRectGhost.push_back(dot);
                    boundaryRectPacman.push_back(dot);
                }
                hEdge.x += offset;
            }
        }   
        else {
            for(int j = 0; j < dimension; j ++) {
                window->renderTexture(texture, &hEdge, &hEdge);
                boundaryRectGhost.push_back(hEdge);
                boundaryRectPacman.push_back(hEdge);
                dot.x += offset;
                window->renderTexture(texture, &dot, &dot);
                boundaryRectGhost.push_back(dot);
                boundaryRectPacman.push_back(dot);
                hEdge.x += offset;
            }

        }
        vEdge.x = startPoint.x;
        vEdge.y += offset;
    }
}

void Maze::createBasicStructure(Window* window) {
    int offset = dotSize + blockSize;

    // create ghost house
    SDL_Point startPoint = getDotScreenCoordinate(dimension / 2 - 1, dimension / 2 -1);
    SDL_Rect dot = {startPoint.x, startPoint.y, dotSize, dotSize},
             vEdge = {startPoint.x, startPoint.y + dotSize, dotSize, blockSize},
             hEdge = {startPoint.x + dotSize, startPoint.y, blockSize, dotSize},
             block = {startPoint.x + dotSize, startPoint.y + dotSize, blockSize, blockSize};
    SDL_Color dotColor = {0xFF, 0x00, 0x00, 0xFF},   // red 
              edgeColor = {0x00, 0x70, 0x00, 0xFF},  // dark green
              blockColor = {0x00, 0x00, 0x00, 0xFF},  // mix
              doorColor = {0xFF, 0x80, 0xFF, 0xFF};  // light magenta
    // create top left square

    window->renderRect(&block, blockColor);
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.x += offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    dot.y -= offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);

    // create top door
    hEdge.x += offset;
    hEdge.y -= offset;
    window->renderTexture(ghostWallHoriz, NULL, &hEdge);
    boundaryRectPacman.push_back(hEdge);
    dot.x += offset;
    hEdge.x += offset;
    window->renderTexture(ghostWallHoriz, NULL, &hEdge);
    window->renderTexture(ghostWallHoriz, NULL, &dot);

    
    boundaryRectPacman.push_back(dot);
    boundaryRectPacman.push_back(hEdge);

    dot.x += offset;
    hEdge.x += offset;
    vEdge.x += 2 * offset;
    block.x += 3 * offset;
    // create top right square
    
    window->renderRect(&block, blockColor);
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.x += offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    dot.y -= offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);

    // create right door
    vEdge.y += offset;
    window->renderTexture(ghostWallVert, NULL, &vEdge);

    boundaryRectPacman.push_back(vEdge);
    dot.y += 2 * offset;
    vEdge.y += offset;
    window->renderTexture(ghostWallVert, NULL, &vEdge);
    window->renderTexture(ghostWallVert, NULL, &dot);
    
    boundaryRectPacman.push_back(dot);
    boundaryRectPacman.push_back(vEdge);

    dot.x -= offset;
    dot.y += offset;
    hEdge.y += 2 * offset;
    vEdge.y += offset;
    vEdge.x -= offset;
    block.y += 3 * offset;
    // create bottom right square
    
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    window->renderRect(&block, blockColor);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.x += offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    dot.y -= offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);

    // create bottom door
    hEdge.x -= offset;
    window->renderTexture(ghostWallHoriz, NULL, &hEdge);
    boundaryRectPacman.push_back(hEdge);
    dot.y += offset;
    dot.x -= 2 * offset;
    hEdge.x -= offset;
    window->renderTexture(ghostWallHoriz, NULL, &hEdge);
    window->renderTexture(ghostWallHoriz, NULL, &dot);
    
    boundaryRectPacman.push_back(dot);
    boundaryRectPacman.push_back(hEdge);

    dot.x -= 2 * offset;
    dot.y -= offset;
    hEdge.x -= offset;
    hEdge.y -= offset;
    block.x -= 3 * offset;
    vEdge.x -= 4 * offset;
    // create bottom left square
    
    window->renderRect(&block, blockColor);
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.y += offset;
    hEdge.y += offset;
    vEdge.x += offset;
    
    window->renderTexture(texture, &dot, &dot);
    window->renderTexture(texture, &hEdge, &hEdge);
    window->renderTexture(texture, &vEdge, &vEdge);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectPacman.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.x += offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);
    dot.y -= offset;
    window->renderTexture(texture, &dot, &dot);
    boundaryRectGhost.push_back(dot);
    boundaryRectPacman.push_back(dot);

    // create left door
    vEdge.x -= offset;
    vEdge.y -= offset;
    window->renderTexture(ghostWallVert, NULL, &vEdge);
    boundaryRectPacman.push_back(vEdge);
    dot.y -= offset;
    dot.x -= offset;
    vEdge.y -= offset;
    window->renderTexture(ghostWallVert, NULL, &vEdge);
    window->renderTexture(ghostWallVert, NULL, &dot);
    
    boundaryRectPacman.push_back(dot);
    boundaryRectPacman.push_back(vEdge);


    // update all adjacent blocks
    updateBlock(dimension / 2 - 2, dimension / 2 - 2, 0, 2, 2, 2, 2);
    updateBlock(dimension / 2 + 1, dimension / 2 - 2, 0, 2, 2, 2, 2);
    updateBlock(dimension / 2 - 2, dimension / 2 + 1, 0, 2, 2, 2, 2);
    updateBlock(dimension / 2 + 1, dimension / 2 + 1, 0, 2, 2, 2, 2);

    updateBlock(dimension / 2 - 3, dimension / 2 - 2, 3, 0, 0, 2, 0);
    updateBlock(dimension / 2 - 3, dimension / 2 - 1, 3, 0, 0, 1, 0);
    updateBlock(dimension / 2 - 3, dimension / 2, 3, 0, 0, 1, 0);
    updateBlock(dimension / 2 - 2, dimension / 2 - 1, 2, 1, 0, 0, 2);
    updateBlock(dimension / 2 - 2, dimension / 2, 2, 1, 2, 0, 0);
    updateBlock(dimension / 2 - 3, dimension / 2 + 1, 3, 0, 0, 2, 0);

    updateBlock(dimension / 2 + 2, dimension / 2 - 2, 3, 2, 0, 0, 0);
    updateBlock(dimension / 2 + 2, dimension / 2 - 1, 3, 1, 0, 0, 0);
    updateBlock(dimension / 2 + 2, dimension / 2, 3, 1, 0, 0, 0);
    updateBlock(dimension / 2 + 1, dimension / 2 - 1, 2, 0, 0, 1, 2);
    updateBlock(dimension / 2 + 1, dimension / 2, 2, 0, 2, 1, 0);
    updateBlock(dimension / 2 + 2, dimension / 2 + 1, 3, 2, 0, 0, 0);

    updateBlock(dimension / 2 - 2, dimension / 2 - 3, 3, 0, 2, 0, 0);
    updateBlock(dimension / 2 - 1, dimension / 2 - 3, 3, 0, 1, 0, 0);
    updateBlock(dimension / 2, dimension / 2 - 3, 3, 0, 1, 0, 0);
    updateBlock(dimension / 2 - 1, dimension / 2 - 2, 2, 2, 0, 0, 1);
    updateBlock(dimension / 2, dimension / 2 - 2, 2, 0, 0, 2, 1);
    updateBlock(dimension / 2 + 1, dimension / 2 - 3, 3, 0, 2, 0, 0);

    updateBlock(dimension / 2 - 2, dimension / 2 + 2, 3, 0, 0, 0, 2);
    updateBlock(dimension / 2 - 1, dimension / 2 + 2, 3, 0, 0, 0, 1);
    updateBlock(dimension / 2, dimension / 2 + 2, 3, 0, 0, 0, 1);
    updateBlock(dimension / 2 - 1, dimension / 2 + 1, 2, 2, 1, 0, 0);
    updateBlock(dimension / 2, dimension / 2 + 1, 2, 0, 1, 2, 0);
    updateBlock(dimension / 2 + 1, dimension / 2 + 2, 3, 0, 0, 0, 2);


    startPoint = getDotScreenCoordinate(1, dimension / 2 + 1);
    vEdge.x = startPoint.x;
    vEdge.y = startPoint.y + dotSize;
    dot.x = startPoint.x;
    dot.y = startPoint.y + offset;
    for(int i = 0; i < 5; i ++) {
        
        window->renderTexture(texture, &dot, &dot);
        window->renderTexture(texture, &vEdge, &vEdge);
        boundaryRectGhost.push_back(dot);
        boundaryRectPacman.push_back(dot);
        boundaryRectGhost.push_back(vEdge);
        boundaryRectPacman.push_back(vEdge);
        if(i == 0) {
            updateBlock(i, dimension / 2 - 1, 2, 2, 2, 0, 0);
            updateBlock(i, dimension / 2, 2, 2, 0, 0, 2);
        }
        else {
            updateBlock(i, dimension / 2 - 1, 3, 0, 2, 0, 0);
            updateBlock(i, dimension / 2, 3, 0, 0, 0, 2);    
        }
        vEdge.y += offset;
        dot.y += offset;
    }

    startPoint = getDotScreenCoordinate(dimension - 4, dimension / 2 + 1);
    vEdge.y = startPoint.y + dotSize;
    dot.y = startPoint.y;
    for(int i = 0; i < 5; i ++) {
        
        window->renderTexture(texture, &dot, &dot);
        window->renderTexture(texture, &vEdge, &vEdge);
        boundaryRectGhost.push_back(dot);
        boundaryRectPacman.push_back(dot);
        boundaryRectGhost.push_back(vEdge);
        boundaryRectPacman.push_back(vEdge);
        if(i == 4) {
            updateBlock(dimension - 1, dimension / 2 - 1, 2, 0, 2, 2, 0);
            updateBlock(dimension - 1, dimension / 2, 2, 0, 0, 2, 2);
        }
        else {
            updateBlock(dimension - 5 + i, dimension / 2 - 1, 3, 0, 2, 0, 0);
            updateBlock(dimension - 5 + i, dimension / 2, 3, 0, 0, 0, 2);    
        }
        vEdge.y += offset;
        dot.y += offset;
    }

    startPoint = getDotScreenCoordinate(dimension / 2 + 1, 1);
    hEdge.x = startPoint.x + dotSize;
    hEdge.y = startPoint.y;
    dot.x = startPoint.x + offset;
    dot.y = startPoint.y;
    for(int i = 0; i < 5; i ++) {
        
        window->renderTexture(texture, &dot, &dot);
        window->renderTexture(texture, &hEdge, &hEdge);
        boundaryRectGhost.push_back(dot);
        boundaryRectPacman.push_back(dot);
        boundaryRectGhost.push_back(hEdge);
        boundaryRectPacman.push_back(hEdge);
        if(i == 0) {
            updateBlock(dimension / 2 - 1, i, 2, 0, 0, 2, 2);
            updateBlock(dimension / 2, i, 2, 2, 0, 0, 2);
        }
        else {
            updateBlock(dimension / 2 - 1, i, 3, 0, 0, 2, 0);
            updateBlock(dimension / 2, i, 3, 2, 0, 0, 0);    
        }
        hEdge.x += offset;
        dot.x += offset;
    }

    startPoint = getDotScreenCoordinate(dimension / 2 + 1, dimension - 4);
    hEdge.x = startPoint.x + dotSize;
    dot.x = startPoint.x;
    for(int i = 0; i < 5; i ++) {
        
        window->renderTexture(texture, &dot, &dot);
        window->renderTexture(texture, &hEdge, &hEdge);
        boundaryRectGhost.push_back(dot);
        boundaryRectPacman.push_back(dot);
        boundaryRectGhost.push_back(hEdge);
        boundaryRectPacman.push_back(hEdge);
        if(i == 4) {
            updateBlock(dimension / 2 - 1, dimension - 1, 2, 0, 2, 2, 0);
            updateBlock(dimension / 2, dimension - 1, 2, 2, 2, 0, 0);
        }
        else {
            updateBlock(dimension / 2 - 1, dimension - 5 + i, 3, 0, 0, 2, 0);
            updateBlock(dimension / 2, dimension - 5 + i, 3, 2, 0, 0, 0);    
        }
        hEdge.x += offset;
        dot.x += offset;
    }

    // create pacman house
    
    startPoint = getDotScreenCoordinate(1, 1);
    hEdge.x = startPoint.x + dotSize;
    hEdge.y = startPoint.y + offset;
    vEdge.x = startPoint.x + offset;
    vEdge.y = startPoint.y + dotSize;
    dot.x = vEdge.x;
    dot.y = hEdge.y;
    dotColor.r = 0x00;
    dotColor.g = 0xF0;
    dotColor.b = 0xFF;
    edgeColor.r = 0x00;
    edgeColor.g = 0xF0;
    edgeColor.b = 0xFF;
    window->renderTexture(pacWallHoriz, NULL, &hEdge);
    window->renderTexture(pacWallVert, NULL, &vEdge);
    window->renderTexture(pacWallHoriz, NULL, &dot);

    
    boundaryRectGhost.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);

    startPoint = getDotScreenCoordinate(dimension, dimension);
    hEdge.x = startPoint.x + dotSize;
    hEdge.y = startPoint.y;
    vEdge.x = startPoint.x;
    vEdge.y = startPoint.y + dotSize;
    dot.x = vEdge.x;
    dot.y = hEdge.y;
    dotColor.r = 0x00;
    dotColor.g = 0xF0;
    dotColor.b = 0xFF;
    edgeColor.r = 0x00;
    edgeColor.g = 0xF0;
    edgeColor.b = 0xFF;

    window->renderTexture(pacWallHoriz, NULL, &hEdge);
    window->renderTexture(pacWallVert, NULL, &vEdge);
    window->renderTexture(pacWallHoriz, NULL, &dot);
    
    boundaryRectGhost.push_back(dot);
    boundaryRectGhost.push_back(hEdge);
    boundaryRectGhost.push_back(vEdge);

    //update blocks
    updateBlock(0, 0, 0, 2, 3, 3, 2);
    updateBlock(0, 1, 2, 2, 0, 0, 3);
    updateBlock(1, 0, 2, 3, 0, 0, 2);

    updateBlock(dimension - 1, dimension - 1, 0, 3, 2, 2, 3);
    updateBlock(dimension - 2, dimension - 1, 2, 0, 2, 3, 0);
    updateBlock(dimension - 1, dimension - 2, 2, 0, 3, 2, 0);
}


void Maze::createEdge(Window* window, int i, int j, int side) {
    SDL_Point point1 = getDotScreenCoordinate(i, j);
    SDL_Point point2 = getDotScreenCoordinate(dimension - i + 2, dimension - j + 2);
    SDL_Rect dot1 = {point1.x, point1.y, dotSize, dotSize},
             dot2 = {point2.x, point2.y, dotSize, dotSize},
             vEdge = {point1.x, point1.y + dotSize, dotSize, blockSize},
             hEdge = {point1.x + dotSize, point1.y, blockSize, dotSize};
    SDL_Color dotColor = {0xFF, 0x00, 0x00, 0xFF},    // red
              edgeColor = {0x00, 0x70, 0x00, 0xFF};   // dark green

    int offset = dotSize + blockSize;
    
    window->renderTexture(texture, &dot1, &dot1);
    window->renderTexture(texture, &dot2, &dot2);
    boundaryRectGhost.push_back(dot1);
    boundaryRectPacman.push_back(dot1);
    boundaryRectGhost.push_back(dot2);
    boundaryRectPacman.push_back(dot2);
    switch(side) {
        case 0:
            vEdge.y = dot1.y - blockSize;
            window->renderTexture(texture, &vEdge, &vEdge);
            boundaryRectGhost.push_back(vEdge);
            boundaryRectPacman.push_back(vEdge);
            dot1.y -= offset;
            window->renderTexture(texture, &dot1, &dot1);
            boundaryRectGhost.push_back(dot1);
            boundaryRectPacman.push_back(dot1);
            break;
        case 1:
            hEdge.x = dot1.x + dotSize;
            window->renderTexture(texture, &hEdge, &hEdge);
            boundaryRectGhost.push_back(hEdge);
            boundaryRectPacman.push_back(hEdge);
            dot1.x += offset;
            window->renderTexture(texture, &dot1, &dot1);
            boundaryRectGhost.push_back(dot1);
            boundaryRectPacman.push_back(dot1);
            break;
        case 2:
            vEdge.y = dot1.y + dotSize;
            window->renderTexture(texture, &vEdge, &vEdge);
            boundaryRectGhost.push_back(vEdge);
            boundaryRectPacman.push_back(vEdge);
            dot1.y += offset;
            window->renderTexture(texture, &dot1, &dot1);
            boundaryRectGhost.push_back(dot1);
            boundaryRectPacman.push_back(dot1);
            break;
        case 3:
            hEdge.x = dot1.x - blockSize;
            window->renderTexture(texture, &hEdge, &hEdge);
            boundaryRectGhost.push_back(hEdge);
            boundaryRectPacman.push_back(hEdge);
            dot1.x -= offset;
            window->renderTexture(texture, &dot1, &dot1);
            boundaryRectGhost.push_back(dot1);
            boundaryRectPacman.push_back(dot1);
            break;
    }
    vEdge.x = point2.x;
    hEdge.y = point2.y;
    //create mirror edge
    switch(side) {
        case 0:
            vEdge.y = dot2.y + dotSize;
            window->renderTexture(texture, &vEdge, &vEdge);
            boundaryRectGhost.push_back(vEdge);
            boundaryRectPacman.push_back(vEdge);
            dot2.y += offset;
            window->renderTexture(texture, &dot2, &dot2);
            boundaryRectGhost.push_back(dot2);
            boundaryRectPacman.push_back(dot2);
            break;
        case 1:
            hEdge.x = dot2.x - blockSize;
            window->renderTexture(texture, &hEdge, &hEdge);
            boundaryRectGhost.push_back(hEdge);
            boundaryRectPacman.push_back(hEdge);
            dot2.x -= offset;
            window->renderTexture(texture, &dot2, &dot2);
            boundaryRectGhost.push_back(dot2);
            boundaryRectPacman.push_back(dot2);
            break;
        case 2:
            vEdge.y = dot2.y - blockSize;
            window->renderTexture(texture, &vEdge, &vEdge);
            boundaryRectGhost.push_back(vEdge);
            boundaryRectPacman.push_back(vEdge);
            dot2.y -= offset;
            window->renderTexture(texture, &dot2, &dot2);
            boundaryRectGhost.push_back(dot2);
            boundaryRectPacman.push_back(dot2);
            break;
        case 3:
            hEdge.x = dot2.x + dotSize;
            window->renderTexture(texture, &hEdge, &hEdge);
            boundaryRectGhost.push_back(hEdge);
            boundaryRectPacman.push_back(hEdge);
            dot2.x += offset;
            window->renderTexture(texture, &dot2, &dot2);
            boundaryRectGhost.push_back(dot2);
            boundaryRectPacman.push_back(dot2);
            break;
    }

}

void Maze::generateMazeRandom(Window* window) {

    int mirrorI, mirrorJ, side, deg1 = 1, deg2 = 1;
    for(int i = 2; i <= dimension; i ++) {
        for(int j = 2; j <= dimension / 2; j ++) {
            if(i==dimension/2+1||j==dimension/2+1)
                continue;
            if(dimension/2-1<=i&&i<=dimension/2+3&&dimension/2-1<=j&&j<=dimension/2+3)
                continue;
            mirrorI = dimension - i + 2;
            mirrorJ = dimension - j + 2;
            bool flag = false;
            int counter = 0;
            int extra;
            side = rand() % 4;
            deg1 = rand() % 2 + 1;
            deg2 = rand() % 2 + 1;
            while(!flag && counter < 16) {
                
                switch(side) {
                    case 0: 
                        if((dimension/2-1<=j&&j<=dimension/2+3&&i==dimension/2+4)) {
                            side = rand() % 4;
                            break;
                        }
                        if(maze[i-2][j-2].degree <= deg1 || maze[i-2][j-1].degree <= deg2){
                            side = 2;
                            break;
                        }
                        else {
                            if(maze[i-2][j-2].right == ACCESS_ALLOWED) {
                                maze[i-2][j-2].right = ALL_DENIED;
                                maze[i-2][j-2].degree --;
                                maze[i-2][j-1].left = ALL_DENIED;
                                maze[i-2][j-1].degree --;
                                // update mirror blocks
                                maze[mirrorI-1][mirrorJ-2].right = ALL_DENIED;
                                maze[mirrorI-1][mirrorJ-2].degree --;
                                maze[mirrorI-1][mirrorJ-1].left = ALL_DENIED;
                                maze[mirrorI-1][mirrorJ-1].degree --;
                                createEdge(window, i, j, 0);
                            }
                            flag = true;
                            break;
                        }
                    case 1:
                        if((dimension/2-1<=i&&i<=dimension/2+3&&j==dimension/2-2)){
                            side = rand() % 4;
                            break;
                        }
                        if(maze[i-1][j-1].degree <= deg1 || maze[i-2][j-1].degree <= deg2){
                            side = 3;
                            break;
                        }
                        else {
                            if(maze[i-2][j-1].down == ACCESS_ALLOWED) {
                                maze[i-2][j-1].down = ALL_DENIED;
                                maze[i-2][j-1].degree --;
                                maze[i-1][j-1].up = ALL_DENIED;
                                maze[i-1][j-1].degree --;
                                // update mirror blocks
                                maze[mirrorI-2][mirrorJ-2].down = ALL_DENIED;
                                maze[mirrorI-2][mirrorJ-2].degree --;
                                maze[mirrorI-1][mirrorJ-2].up = ALL_DENIED;
                                maze[mirrorI-1][mirrorJ-2].degree --;
                                createEdge(window, i, j, 1);
                            }
                            flag = true;
                            break;
                        }  
                    case 2:
                        if((dimension/2-1<=j&&j<=dimension/2+3&&i==dimension/2-2)) {
                            side = rand() % 4;
                            break;
                        }
                        if(maze[i-1][j-2].degree <= deg1 || maze[i-1][j-1].degree <= deg2) {
                            side = 0;
                            break;
                        }
                        else {
                            if(maze[i-1][j-2].right == ACCESS_ALLOWED) {
                                maze[i-1][j-2].right = ALL_DENIED;
                                maze[i-1][j-2].degree --;
                                maze[i-1][j-1].left = ALL_DENIED;
                                maze[i-1][j-1].degree --;
                                // update mirror blocks
                                maze[mirrorI-2][mirrorJ-2].right = ALL_DENIED;
                                maze[mirrorI-2][mirrorJ-2].degree --;
                                maze[mirrorI-2][mirrorJ-1].left = ALL_DENIED;
                                maze[mirrorI-2][mirrorJ-1].degree --;
                                createEdge(window, i, j, 2);
                            }
                            flag = true;
                            break;
                        }
                    case 3:
                        if((dimension/2-1<=i&&i<=dimension/2+3&&j==dimension/2+4)){
                            side = rand() % 4;
                            break;
                        }
                        if(maze[i-1][j-2].degree <= deg1 || maze[i-2][j-2].degree <= deg2){
                            side = 1;
                            break;
                        }
                        else {
                            if(maze[i-2][j-2].down == ACCESS_ALLOWED) {
                                maze[i-2][j-2].down = ALL_DENIED;
                                maze[i-2][j-2].degree --;
                                maze[i-1][j-2].up = ALL_DENIED;
                                maze[i-1][j-2].degree --;
                                // update mirror blocks
                                maze[mirrorI-2][mirrorJ-1].down = ALL_DENIED;
                                maze[mirrorI-2][mirrorJ-1].degree --;
                                maze[mirrorI-1][mirrorJ-1].up = ALL_DENIED;
                                maze[mirrorI-1][mirrorJ-1].degree --;
                                createEdge(window, i, j, 3);
                            }
                            flag = true;
                            break;
                        }                      
                }
                counter ++;
            }    
        } 
    }
} 