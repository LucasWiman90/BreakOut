#ifndef POWERUP_H
#define POWERUP_H

#include "constants.h"
#include "entity.h"

//Class to represent a powerup
//Inherits from entity
//A powerup 

class powerup: public moving_entity {

    static sf::Texture texture;

    std::string type;

public:

    powerup(float x, float y, const std::string& type);

    // Implement the helper functions
    void move_up() noexcept override;
    void move_left() noexcept override;
    void move_right() noexcept override;
    void move_down() noexcept override;

    void update() override;
    void draw(sf::RenderWindow& window) override;

    std::string get_type() const;
};

#endif