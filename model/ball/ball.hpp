#ifndef BALL_HPP
#define BALL_HPP

#include "../point/point.hpp"
#include <math.h>

// NOTE: might wanna get inspo from translation class in labs to do the
// increased-speed nerf

enum class BounceType { horizontal, vertical };

class Ball {
  private:
    Point coord_;
    Point direction_;
    unsigned speed_;

  public:
    Ball(Point coord, Point direction, unsigned speed = 1);

    virtual ~Ball() = default;

    virtual Point getCoordinate();
    virtual void setSpeed(unsigned speed);
    virtual void setDirection(Point newDirection);

    virtual void update();
    virtual void bounce(BounceType bounceType);
};

#endif
