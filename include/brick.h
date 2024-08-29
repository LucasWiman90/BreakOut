#ifndef BRICK_H
#define BRICK_H

#include "constants.h"
#include "entity.h"

//Class to represent a brick
//Inherits from entity
//A grid of bricks is displayed across the top of the screen
//This grid will be represented by an std::vector of brick objects
//When the ball hits a brick, the brick is destroyed and disappears
//When a brick is destroyed, it is deleted from the vector

class brick: public entity {

    static sf::Texture texture;

    int strength{constants::brick_strength};
public:

    brick(float x, float y);

    //Helper functions for brick strength
    void set_strength(int) noexcept;
    void weaken() noexcept;
    bool is_too_weak() const noexcept;

    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif