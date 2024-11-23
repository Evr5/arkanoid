#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "../ball/ball.hpp"
#include "../brick/brick.hpp"

#include <array>
#include <memory>
#include <vector>

constexpr double boardHeight = 10;
constexpr double boardWidth = 10;
constexpr double boardBoundingsThickness = 10;

class GameBoard {
  private:
    std::vector<std::shared_ptr<Ball>> balls_ = {
        std::make_shared<Ball>(Vec2{5, 5}, Vec2{1, 1}, 1)};

    std::vector<std::shared_ptr<Brick>> bricks_{
        // Brick::makeBrick(Color::red, BoundingBox{Vec2{6, 6}, Vec2{11, 2}})
    };

    // with T=thickness, H=height, W=width
    std::array<BoundingBox, 3> borders_ = {
        // (-T, H + T) -> (0,0)
        BoundingBox{Vec2{-boardBoundingsThickness,
                         boardHeight + boardBoundingsThickness},
                    Vec2{0, 0}},

        // (0, H + T) -> (W,H)
        BoundingBox{Vec2{0, boardHeight + boardBoundingsThickness},
                    Vec2{boardWidth, boardHeight}},

        // (W, H + T) -> (W + T, 0)
        BoundingBox{Vec2{boardWidth, boardHeight + boardBoundingsThickness},
                    Vec2{boardWidth + boardBoundingsThickness, 0}}};

    // Racket racket;

  public:
    GameBoard() = default;
    virtual ~GameBoard() = default;

    void update(double deltaTime);
};

#endif
