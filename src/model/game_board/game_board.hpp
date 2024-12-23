#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "../ball/ball.hpp"
#include "../bonus/bonus.hpp"
#include "../bonus/bonus_pill.hpp"
#include "../border/border.hpp"
#include "../brick/brick.hpp"
#include "../life_counter//life_counter.hpp"
#include "../racket/racket.hpp"
#include "../score_manager/score_manager.hpp"

#include <memory>
#include <optional>
#include <variant>
#include <vector>

// TODO: move this to global constants

using BrickIt = std::vector<std::shared_ptr<Brick>>::const_iterator;
using BorderIt = std::vector<std::shared_ptr<Border>>::const_iterator;

// TODO: move to or from globalVariables.hpp to avoid splitting variables
// everywhere

class GameBoard final {
  private:
    ScoreManager scoreManager_;
    LifeCounter lifeCounter_;
    int bestScore_ = 0;
    unique_ptr<AbstractTimedBonus> activeBonus_;
    std::vector<unique_ptr<BonusPill>> descendingBonusses_;

    std::shared_ptr<Racket> racket_;
    std::vector<std::shared_ptr<Border>> borders_;
    std::vector<std::shared_ptr<Brick>> bricks_;
    std::vector<std::shared_ptr<Ball>> balls_;

    /**
     * @brief Finds the next collision involving the specified ball.
     *
     * @param ball Reference to the ball involved in the collision.
     * @return An optional variant containing either Brick-iterator or a
     * Border-iterator or a shared_ptr<Racket> corresponding to the next
     * Colliding object. The optional has no value if no collisions were found.
     */
    std::optional<std::variant<BrickIt, BorderIt, shared_ptr<Racket>>>
    findNextCollision(Ball &ball);

    /**
     * @brief Resolves collisions involving the specified ball and
     * calculates the points earned from this collision resolution.
     *
     * @param ball Reference to the ball involved in the collision.
     * @return The number of points earned as a result of resolving the
     * collisions.
     */
    size_t solveBallCollisions(Ball &ball);

    void applyBonus(BonusType bonusType);

    void undoBonusEffect(BonusType bonusType);

  public:
    /**
     * @brief Default constructor.
     */
    GameBoard() = default;

    /**
     * @brief Default destructor.
     */
    ~GameBoard() = default;

    /**
     * @brief Updates the GameBoard state based on elapsed time.
     *
     * @param deltaTime Time elapsed (in seconds) since the last update.
     */
    void update(double deltaTime);

    // #### Getters ####

    /**
     * @brief Returns the current Score.
     *
     * @return The current score.
     */
    unsigned long getScore() const;

    /**
     * @brief Returns a reference to the lifeCounter.
     *
     * @return A reference to the lifeCounter.
     */
    const LifeCounter &getLife() const;

    /**
     * @brief Returns the current number of bricks.
     *
     * @return The current number of bricks.
     */
    unsigned long getNumBricks() const;

    /**
     * @brief Returns the currently active Balls.
     *
     * @return A reference to the vector of ball pointers.
     */
    const std::vector<std::shared_ptr<Ball>> &getBalls() const;

    /**
     * @brief Returns the current Bricks.
     *
     * @return A reference to the vector of brick pointers.
     */
    const std::vector<std::shared_ptr<Brick>> &getBricks() const;

    /**
     * @brief Returns the racket.
     *
     * @return A reference to the racket pointer.
     */
    const std::shared_ptr<Racket> &getRacket() const;

    /**
     * @brief Returns the map borders.
     *
     * @return A reference to the vector of border pointers.
     */
    const std::vector<std::shared_ptr<Border>> &getBorders() const;
    int getBestScore() const;

    // #### Setters ####

    /**
     * @brief Sets the racket's horizontal cooddinate.
     *
     * @param posX The racket's horizontal cooddinate.
     */
    void setRacketAtX(double posX);

    /**
     * @brief Sets the balls.
     *
     * @param balls A reference to the vector of brick pointers.
     */
    void setBalls(const std::vector<std::shared_ptr<Ball>> &balls);

    /**
     * @brief Sets the bricks.
     *
     * @param bricks A reference to the vector of brick pointers.
     */
    void setBricks(const std::vector<std::shared_ptr<Brick>> &bricks);

    /**
     * @brief Sets the racket.
     *
     * @param bricks A reference to the racket pointer.
     */
    void setRacket(const std::shared_ptr<Racket> &racket);

    /**
     * @brief Sets the borders.
     *
     * @param borders A reference to the vector of border pointers.
     */
    void setBorders(const std::vector<std::shared_ptr<Border>> &borders);

    void resetLifeCounter();

    void resetScore();

    void saveRecordScore();

    // #### Get the score from the file ####
    void readBestScore();

    void resetBestScore();

    // #### Clear GameBoard ####

    void clearBalls();

    void clearBonus();

    void clearDescendingBonusses();

    void clearBorders();

    void clearBricks();

    void clear();

    // ### Pause the game ###
    void pauseGameBoard();
};

#endif
