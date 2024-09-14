#ifndef POWERUP_H
#define POWERUP_H

#include "constants.h"
#include "entity.h"

//Class to represent a powerup
//Inherits from entity
//A powerup 

class powerup: public moving_entity {

public:

    // Enum to represent the type of powerup
    enum class Type {
        None,       //No active powerup
        Paddle,
        Ball
    };

    powerup(float x, float y, Type type, std::string name);

    // Implement the helper functions
    void move_up() noexcept override;
    void move_left() noexcept override;
    void move_right() noexcept override;
    void move_down() noexcept override;

    void update() override;
    void draw(sf::RenderWindow& window) override;

    Type get_type() const;
    std::string get_name() const;

private:
    static sf::Texture texture;
    Type type;
    std::string name;
};

#endif