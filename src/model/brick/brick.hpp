#ifndef BRICK_HPP
#define BRICK_HPP

#include "../../global_variables.hpp"
#include "../bonus_type/bonus_type.hpp"
#include "../bounceable/bounceable.hpp"
#include <memory>

/**
 * @brief Contains all the brick colors and its score associated.
 */
enum class Color : size_t { // Score/Points is always positive
    defaultBrick = 1,
    white = 50,
    orange = 60,
    cyan = 70,
    green = 80,
    red = 90,
    blue = 100,
    magenta = 110,
    yellow = 120,
    silver = 200,
    gold
};

class Brick : public Bounceable {
  protected:
    Color color_;
    uint8_t durability_;
    BonusType bonusType_;

  protected:
    /**
     * @brief Constructs a new Brick.
     * @param center The brick's center.
     * @param width The brick's width.
     * @param height The brick's height.
     * @param color The brick's color.
     * @param durability The brick's durability.
     * @param bonusType The bonus held contained in the brick.
     */
    Brick(const Vec2 &center, double width, double height, Color color,
          uint8_t durability = DURABILITY_STANDARD_BRICK,
          BonusType bonusType = BonusType::None);

    Brick(const Brick &) = default;
    Brick(Brick &&) = delete;
    Brick &operator=(const Brick &) = delete;
    Brick &operator=(Brick &&) = delete;

  public:
    virtual ~Brick();

    /**
     * @brief Brick factory. Returns a unique pointer to a new Brick.
     * @param center The brick's center.
     * @param width The brick's width.
     * @param height The brick's height.
     * @param color The brick's color.
     * @param bonusType The bonus held contained in the brick.
     */
    static std::unique_ptr<Brick>
    makeBrick(const Vec2 &center, double width, double height, Color color,
              BonusType bonusType = BonusType::None);

    /**
     * @brief Hit the brick. Returns the type of bonus held inside of the brick
     * is destroyed.
     */
    virtual BonusType hit();

    /**
     * @brief Returns the brick's color.
     */
    virtual Color getColor() const;

    /**
     * @brief Returns the score awarded for destroying the brick.
     */
    virtual size_t getScore() const;

    /**
     * @brief Returns the brick's durability (how many times to hit it left
     * before it should be destroyed).
     */
    virtual uint8_t getDurability() const;

    /**
     * @brief Returns the brick's durability (hits remaining before
     * destruction).
     */
    virtual bool isDestroyed() const;

    /**
     * @brief Returns the type of bonus contained in the brick.
     */
    virtual BonusType getBonusType() const;

    /**
     * @brief Returns true if the brick contains a bonus.
     */
    virtual bool hasBonus() const;

    /**
     * @brief Returns a pointer to a cloned brick.
     */
    virtual std::shared_ptr<Brick> clone() = 0;
};

#endif
