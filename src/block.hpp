#pragma once

enum BlockAccess{
    ACCESS_ALLOWED,
    PACMAN_DENIED,
    ALL_DENIED
};

class Block {
    
    public:
    Block();
    Block(int degree, int up, int right, int down, int left);

    int degree;
    int up, right, down, left;
};

Block::Block() {
    degree = 4;
    up = ACCESS_ALLOWED;
    right = ACCESS_ALLOWED;
    down = ACCESS_ALLOWED;
    left = ACCESS_ALLOWED;
}

Block::Block(int degree, int up, int right, int down, int left) {
    this->degree = degree;
    this->up = up;
    this->right = right;
    this->down = down;
    this->left = left;    
}
