#ifndef BALL_H
#define BALL_H

#include "constants.h"
#include "entity.h"
#include "powerup.h"

//Class to represent the bouncing ball
class ball: public moving_entity {
    static sf::Texture texture;
    bool firemode;

public:
    //Interface of the class
    ball(float x, float y);

    // Implement the helper functions
    void move_up() noexcept override;
    void move_left() noexcept override;
    void move_right() noexcept override;
    void move_down() noexcept override;

    void apply_powerup(powerup& p);

    bool isFireMode() const;
    void set_firemode(bool mode);

    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif