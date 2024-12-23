#include "game_board.hpp"
#include "../../log/log.hpp"

#include <algorithm>
#include <memory>
#include <sys/types.h>

std::optional<std::variant<BrickIt, BorderIt, std::shared_ptr<Racket>>>
GameBoard::findNextCollision(Ball &ball) {
    std::optional<std::variant<BrickIt, BorderIt, std::shared_ptr<Racket>>>
        closestCollision;
    double distanceClosestCollision = std::numeric_limits<double>::max();

    auto checkCollisions = [&](auto &elements, auto &closestCollision) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (ball.checkCollision((*it)->getBoundingBox())) {
                double distanceCurrentCollision =
                    ball.getUnidirectionalPenetration((*it)->getBoundingBox())
                        .getModule();

                if (distanceCurrentCollision < distanceClosestCollision) {
                    closestCollision = it;
                    distanceClosestCollision = distanceCurrentCollision;
                }
            }
        }
    };

    checkCollisions(bricks_, closestCollision);
    checkCollisions(borders_, closestCollision);

    if (racket_ && ball.checkCollision(racket_->getBoundingBox())) {
        double distanceCurrentCollision =
            ball.getUnidirectionalPenetration(racket_->getBoundingBox())
                .getModule();

        if (distanceCurrentCollision < distanceClosestCollision) {
            closestCollision = racket_;
            distanceClosestCollision = distanceCurrentCollision;
        }
    }

    return closestCollision;
}

size_t GameBoard::solveBallCollisions(Ball &ball) {
    size_t pointsEarned = 0;
    bool collided = true;
    do {
        auto collidingObject = (findNextCollision(ball));

        collided = collidingObject.has_value();
        if (!collided) {
            break;
        }

        if (std::holds_alternative<shared_ptr<Racket>>(
                collidingObject.value())) {
            Log::get().addMessage(Log::LogType::CollidingObject, "racket");
            shared_ptr<Racket> racket =
                std::get<shared_ptr<Racket>>(*collidingObject);
            ball.collide(*racket);

        } else if (std::holds_alternative<BrickIt>(collidingObject.value())) {
            Log::get().addMessage(Log::LogType::CollidingObject, "brick");
            BrickIt brickIt = std::get<BrickIt>(*collidingObject);
            ball.collide(*brickIt->get());
            BonusType bonusType =
                (*brickIt)->hit(); // decrement its durability and extract bonus
            if ((*brickIt)->isDestroyed()) { // erase it if destroyed
                Log::get().addMessage(
                    Log::LogType::BrickDestroyed,
                    std::string{"Brick at "}
                        + string{(*brickIt)->getBoundingBox().getCenter()});

                // add The Bonus Pill to the descendingBonusses_ vec

                if (bonusType != BonusType::None) {
                    // vertical space between brick and pill centers
                    double verticalSpace = ((*brickIt)->getHeight() / 2)
                                           - (BONUS_PILL_HEIGHT / 2.0);
                    Vec2 brickCenter = (*brickIt)->getCenter();
                    Vec2 bonusPillCenter{brickCenter.x,
                                         brickCenter.y - verticalSpace};

                    cout << "pushing descending bonus" << endl;
                    descendingBonusses_.emplace_back(
                        std::make_unique<BonusPill>(bonusType,
                                                    bonusPillCenter));
                }

                pointsEarned += (*brickIt)->getScore();
                bricks_.erase(brickIt);
            }

        } else if (std::holds_alternative<BorderIt>(collidingObject.value())) {
            Log::get().addMessage(Log::LogType::CollidingObject, "border");
            BorderIt borderIt = std::get<BorderIt>(*collidingObject);
            ball.collide(*borderIt->get());
        }

    } while (collided);

    for (auto descendingBonusIt = descendingBonusses_.begin();
         descendingBonusIt != descendingBonusses_.end();) {

        cout << (*descendingBonusIt)->getCoordinate() << " | "
             << racket_->getCoordinate() << endl;

        if ((*descendingBonusIt)->checkCollision(racket_->getBoundingBox())) {
            cout << "colliding" << endl;
            BonusType bonusType = (*descendingBonusIt)->getBonusType();
            applyBonus(bonusType);
            descendingBonusses_.erase(descendingBonusIt);
        } else {
            descendingBonusIt++;
        }
    }

    return pointsEarned;
}

void GameBoard::applyBonus(BonusType bonusType) {
    switch (bonusType) {
    case BonusType::SlowDown: {
        if (activeBonus_ != nullptr
            && activeBonus_->getBonusType() == BonusType::SlowDown) {
            activeBonus_->reapply();
        } else {
            activeBonus_ = make_unique<SlowDownBonus>();
        }

        double slowDownFactor =
            dynamic_cast<SlowDownBonus *>(activeBonus_.get())
                ->getSlowDownFactor();

        for (auto &ball : balls_) {
            ball->setSpeed(BALL_SPEED / slowDownFactor);
        }
        break;
    }
    case BonusType::WideRacket:
        activeBonus_ = make_unique<BasicTimedBonus>(BonusType::WideRacket);
        racket_->setWidth(WIDE_RACKET_WIDTH);
        break;

    case BonusType::ExtraLife:
        ++lifeCounter_;
        break;

    default:
        break;
    }
}

void GameBoard::undoBonusEffect(BonusType bonusType) {
    switch (bonusType) {

    case BonusType::SlowDown:
        for (auto &ball : balls_) {
            ball->setSpeed(BALL_SPEED);
        }
        break;
    case BonusType::WideRacket:
        racket_->setWidth(RACKET_WIDTH);
        break;
    default:
        break;
    }

    activeBonus_.reset();
}

void GameBoard::update(double deltaTime) {
    if (deltaTime == 0) {
        return;
    }

    for (unique_ptr<BonusPill> &descendingBonus : descendingBonusses_) {
        descendingBonus->update(deltaTime);
    }

    for (auto descendingBonusIt = descendingBonusses_.begin();
         descendingBonusIt != descendingBonusses_.end();) {
        (*descendingBonusIt)->update(deltaTime);
        if ((*descendingBonusIt)->getCoordinate().y < 0) {
            descendingBonusses_.erase(descendingBonusIt);
        } else {
            descendingBonusIt++;
        }
    }

    if (activeBonus_ != nullptr) {
        bool isActive = activeBonus_->update(deltaTime);
        BonusType bonusType = activeBonus_->getBonusType();
        if (bonusType == BonusType::SlowDown) {
            double slowDownFactor =
                dynamic_cast<SlowDownBonus *>(activeBonus_.get())
                    ->getSlowDownFactor();

            for (auto &ball : balls_) {
                ball->setSpeed(BALL_SPEED / slowDownFactor);
            }
        }

        if (!isActive) {
            undoBonusEffect(bonusType);
            activeBonus_.reset();
        }
    }

    for (auto &ball : balls_) {
        Log::get().addMessage(Log::LogType::BallPos, ball->getCoordinate());

        size_t scoreToAdd = solveBallCollisions(*ball);
        scoreManager_.increaseScore(scoreToAdd);

        if (ball->getCoordinate().y < ball->getRadius() / 2) {
            balls_.erase(std::find(balls_.begin(), balls_.end(), ball));
            --lifeCounter_;
            if (lifeCounter_ > 0) { // TODO : VERY BAD, magic numbers
                                    // everywhere. check si on fait ca ici ? y a
                                    // un check de vie dans le controller
                balls_.emplace_back(std::make_shared<Ball>(
                    Vec2{BOARD_WIDTH / 2 + WALL_THICKNESS - 1, 85}, Vec2{0, 1},
                    BALL_RADIUS, BALL_SPEED));
            }
        }
        ball->update(deltaTime);
    }
}

void GameBoard::saveRecordScore() { scoreManager_.saveScore(); }

unsigned long GameBoard::getScore() const {
    return scoreManager_.getCurrentScore();
}

const LifeCounter &GameBoard::getLife() const { return lifeCounter_; }

void GameBoard::setRacketAtX(double posX) { racket_->setPosX(posX); }

// #### Getters meant to be used by the View ####

const std::vector<std::shared_ptr<Ball>> &GameBoard::getBalls() const {
    return balls_;
}

const std::vector<std::shared_ptr<Brick>> &GameBoard::getBricks() const {
    return bricks_;
}

const std::shared_ptr<Racket> &GameBoard::getRacket() const { return racket_; }

const std::vector<std::shared_ptr<Border>> &GameBoard::getBorders() const {
    return borders_;
}

unsigned long GameBoard::getNumBricks() const {
    return std::count_if(bricks_.begin(), bricks_.end(), [](const auto &brick) {
        return brick->getColor() != Color::gold;
    });
}

void GameBoard::resetLifeCounter() { lifeCounter_.reset(); } // reset the life

void GameBoard::resetScore() { scoreManager_.resetScore(); } // reset the score

// ### Setters ###
void GameBoard::setBalls(const std::vector<std::shared_ptr<Ball>> &balls) {
    balls_ = balls;
}

void GameBoard::setBricks(const std::vector<std::shared_ptr<Brick>> &bricks) {
    bricks_ = bricks;
}

void GameBoard::setRacket(const std::shared_ptr<Racket> &racket) {
    racket_ = racket;
}

void GameBoard::setBorders(
    const std::vector<std::shared_ptr<Border>> &borders) {
    borders_ = borders;
}

// #### Clear GameBoard ####

void GameBoard::clearBalls() { balls_.clear(); }

void GameBoard::clearBonus() { activeBonus_.reset(); }

void GameBoard::clearBorders() { borders_.clear(); }

void GameBoard::clearBricks() { bricks_.clear(); }

void GameBoard::clear() {
    clearBonus();
    clearBalls();
    clearBorders();
    clearBricks();
}

// ### Get the best score from the file ###
void GameBoard::readBestScore() {
    // TODO: check because the same function in constructor of score_manager
    std::string filePath = "score.txt";

    std::ifstream read(filePath, ios::in);

    if (read.is_open()) {
        read >> bestScore_;
        read.close();
    }
}

int GameBoard::getBestScore() const { // TODO: check because the same function
                                      // in constructor of score_manager
    return bestScore_;
}

void GameBoard::resetBestScore() {
    bestScore_ = 0;
    scoreManager_.setScore0();
}

void GameBoard::pauseGameBoard() {}
