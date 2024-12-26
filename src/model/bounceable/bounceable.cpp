#include "bounceable.hpp"
#include "../vec2/vec2.hpp"

#include <stdexcept>

std::string bounceTypeToString(BounceType bounceType) {
    switch (bounceType) {
    case BounceType::Vertical:
        return "Vertical";
        break;
    case BounceType::Horizontal:
        return "Horizontal";
        break;
    case BounceType::Corner:
        return "Corner";
        break;
    default:
        std::cerr << "Unknown BounceType" << std::endl;
        exit(-1);
    }
}

Bounceable::Bounceable(Vec2 pos, double width, double height)
    : boundingBox_(pos, width, height) {}

Bounceable::Bounceable(Vec2 topLeft, Vec2 bottomRight)
    : boundingBox_(topLeft, bottomRight) {}

Bounceable::Bounceable(const BoundingBox &boundingBox)
    : boundingBox_(boundingBox) {}

Bounceable::~Bounceable() = default;

const BoundingBox &Bounceable::getBoundingBox() const { return boundingBox_; }

BounceType Bounceable::getBounceType(const Vec2 &point) const {
    if (point == boundingBox_.getTopLeft()
        or point == boundingBox_.getTopRight()
        or point == boundingBox_.getBottomLeft()
        or point == boundingBox_.getBottomRight()) {

        return BounceType::Corner;
    } else if (point.x == boundingBox_.getLeft()
               or point.x == boundingBox_.getRight()) {
        return BounceType::Vertical;
    } else if (point.y == boundingBox_.getBottom()
               or point.y == boundingBox_.getTop()) {
        return BounceType::Horizontal;
    } else {
        throw std::runtime_error{
            "point is not on a the Bounding Box's perimeter"};
    }
}

Vec2 Bounceable::getDirVecAfterBounce(const Vec2 &closestPoint,
                                      const Vec2 &dirVec) const {
    BounceType bounceType = getBounceType(closestPoint);

    Vec2 ret = dirVec; // copy dirVec then change its components
    if (bounceType == BounceType::Horizontal) {
        ret.y = -ret.y;
    }
    if (bounceType == BounceType::Vertical) {
        ret.x = -ret.x;
    } else if (bounceType == BounceType::Corner) {
        ret.y = -ret.y;
        ret.x = -ret.x;
    }

    return ret;
}
