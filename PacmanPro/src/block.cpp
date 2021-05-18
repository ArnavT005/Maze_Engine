#include "block.hpp"

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