#include "ball.hpp"

#include "../bounceable/bounceable.hpp"
#include "../bounding_box/bounding_box.hpp"

#include <cmath>
#include <iostream>
#include <ostream>

using Point = Vec2;

Ball::Ball(Vec2 coord, Vec2 directionVec, double radius, double speed)
    : coord_{coord}, dirVec_{directionVec.normalize()}, radius_{radius},
      speed_{speed} {}

Vec2 Ball::getCoordinate() { return coord_; }
void Ball::setSpeed(unsigned speed) { speed_ = speed; };
void Ball::setDirection(const Vec2 &vec) { dirVec_ = vec; }

void Ball::update(double deltaTime) {
    prevCoord_ = coord_;
    coord_ += (dirVec_ * speed_ * deltaTime);
}

// NOTE: this only does the direction vector part of the bounce
// actual bounce logic is in collide function
void Ball::bounce(const Bounceable &bounceable) {
    Point closestPoint = getClosestPoint(bounceable.getBoundingBox());

    dirVec_ = bounceable.getDirVecAfterBounce(closestPoint, dirVec_);
}

void Ball::collide(const Bounceable &bounceable) {
    Vec2 UnidirectionalPenetration =
        getUnidirectionalPenetration(bounceable.getBoundingBox());

    Point closestPoint = getClosestPoint(bounceable.getBoundingBox());
    BounceType bounceType = bounceable.getBounceType(closestPoint);

    Vec2 bidirectionalPenetration;
    Vec2 changeBetweenLastUpdate{coord_ - prevCoord_};

    double penetrationRate = 1; // Defaults to prevent 0 division
    if (bounceType == BounceType::Horizontal
        and (changeBetweenLastUpdate.getY() != 0)) {
        penetrationRate =
            UnidirectionalPenetration.getY() / changeBetweenLastUpdate.getY();

    } else if ((bounceType == BounceType::Vertical
                or bounceType == BounceType::Corner)
               and changeBetweenLastUpdate.getX() != 0) {
        penetrationRate =
            UnidirectionalPenetration.getX() / changeBetweenLastUpdate.getX();
    }

    bidirectionalPenetration = changeBetweenLastUpdate * penetrationRate;

    coord_ -= bidirectionalPenetration;

    bounce(bounceable);

    // add back what the distance that the ball should have gone while it was
    // going inside the bounding-box
    // coord_ += dirVec_ * bidirectionalPenetration.getModule();
}

Vec2 Ball::getUnidirectionalPenetration(const BoundingBox &boundingBox) const {
    // TODO: write a comment to explain how this works
    Point closestPoint =
        getClosestPoint(boundingBox); // Closest point from the ball's center,
                                      // on the bounding-box's edge

    Vec2 coordToClosestPoint{
        closestPoint - coord_}; // Vector from ball's center to closest point

    double angleRad =
        atan2(closestPoint.getY() - coord_.getY(),
              closestPoint.getX()
                  - coord_.getX()); // Angle between closestPoint and the
                                    // 0 degree from the circle's center

    // The point of the circle that is most inscribed in the bounding-box
    Point pointInBoundingBox{coord_.getX() + radius_ * cos(angleRad),
                             coord_.getY() + radius_ * sin(angleRad)};

    // Vector between from ball's center to pointInBoundingBox
    Vec2 coordToPointInBoundingBox = pointInBoundingBox - coord_;

    // Vector from closestPoint to pointInBoundingBox (monodirectional
    // penetration vector)
    Vec2 MonoDirectionalPenetrationVec =
        coordToPointInBoundingBox - coordToClosestPoint;

    return MonoDirectionalPenetrationVec;
}

Vec2 Ball::getClosestPoint(const BoundingBox &boundingBox) const {
    Vec2 boundingBoxHalfExtents{boundingBox.getWidth() / 2,
                                boundingBox.getHeight() / 2};

    Vec2 distance = coord_ - boundingBox.getCenter();

    Vec2 clamped =
        distance.clamped(-boundingBoxHalfExtents, boundingBoxHalfExtents);

    Vec2 closestPoint = clamped + boundingBox.getCenter();

    return closestPoint;
}

bool Ball::hasReached(const Vec2 &point) const {
    float deltaX = point.getX() - coord_.getX();
    float deltaY = point.getY() - coord_.getY();
    std::cout << "delta X : " << deltaX << std::endl;
    return Vec2{deltaX, deltaY}.getModule() < radius_;
}

bool Ball::checkCollision(const BoundingBox &boundingBox) const {
    Vec2 closestVec2 = getClosestPoint(boundingBox);

    return hasReached(closestVec2);
}
