#pragma once

enum BlockAccess{
    ACCESS_ALLOWED,
    PACMAN_DENIED,
    ALL_DENIED,
    GHOST_DENIED
};

class Block {
    
    public:
    Block();
    Block(int degree, int up, int right, int down, int left);

    int degree;
    int up, right, down, left;
};
