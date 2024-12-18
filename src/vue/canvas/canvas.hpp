/**
 * @file canvas.hpp
 * @author Ethan Van Ruyskensvelde (Main developper)
 * @brief Define the class Canvas that will be used to draw all the pieces of
 * the game
 * @date 24/11/2024
 *
 */

#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "../../model/game_board/game_board.hpp"
#include "../ball_renderer/ball_renderer.hpp"
#include "../brick_renderer/brick_renderer.hpp"
#include "../racket_renderer/racket_renderer.hpp"
#include "../wall_renderer/wall_renderer.hpp"

#include <vector>

using namespace std;

// TODO: mettre const

class Canvas {
  private:
    shared_ptr<GameBoard> gameBoard_;
    // TODO: check si on fait ca dans le hpp
    const vector<shared_ptr<Racket>> &rackets_ = gameBoard_->getRackets();
    const vector<shared_ptr<Brick>> &bricks_ = gameBoard_->getBricks();
    const vector<shared_ptr<Ball>> &balls_ = gameBoard_->getBalls();
    const vector<shared_ptr<Border>> &borders_ = gameBoard_->getBorders();
    BallRenderer ballRenderer_;
    BrickRenderer brickRenderer_;
    RacketRenderer racketRenderer_;
    WallRenderer wallRenderer_;

  public:
    /**
     * @brief Construct a new Canvas object
     *
     * @param gameBoard
     */
    Canvas(
        shared_ptr<GameBoard> gameBoard);

    /**
     * @brief Destroy the Canvas object
     * 
     */
    ~Canvas() = default;

    /**
     * @brief Draw all the elements of the game
     *
     */
    void draw();
};

#endif // CANVAS_HPP
