#ifndef GOLD_BRICK_HPP
#define GOLD_BRICK_HPP

#include "brick.hpp"

class GoldBrick : public Brick {
  public:
    GoldBrick(Rectangle rectangle);

    virtual void hit() override; // Gold bricks are never destroyed
};

#endif
