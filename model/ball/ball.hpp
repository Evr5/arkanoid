#ifndef BALL_HPP
#define BALL_HPP

#include "../bounding_box/bounding_box.hpp"
#include "../vec2/vec2.hpp"

#include <math.h>

constexpr double DEFAULT_BALL_RADIUS = 1;
constexpr double DEFAULT_BALL_SPEED = 1;

// NOTE: might wanna get inspo from translation class in labs to do the
// increased-speed nerf

class Ball {
  private:
    Vec2 coord_;
    Vec2 prevCoord_; // coordinate at the previous update
    Vec2 dirVec_;    // direction vector (always normalized)
    double radius_{1};
    double speed_{1};

  public:
    Ball(Vec2 coord, Vec2 directionVec, double radius = DEFAULT_BALL_RADIUS,
         double speed = DEFAULT_BALL_SPEED);

    virtual ~Ball() = default;

    virtual Vec2 getCoordinate();
    virtual void setSpeed(unsigned speed);
    virtual void setDirection(const Vec2 &vec);
    // virtual void move(double distance);

    virtual Vec2 getClosestPoint(const BoundingBox &boundingBox) const;
    virtual bool hasReached(const Vec2 &point) const;
    virtual bool checkCollision(const BoundingBox &boundingBox) const;
    virtual Vec2
    getUnidirectionalPenetration(const BoundingBox &boundingBox) const;

    virtual void bounce(const BoundingBox &boundingBox);
    virtual void collide(const BoundingBox &boundingBox);

    virtual void update(double deltaTime);
};

#endif
