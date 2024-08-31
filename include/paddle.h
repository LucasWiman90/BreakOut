#ifndef PADDLE_H
#define PADDLE_H

#include "constants.h"
#include "entity.h"
#include "powerup.h"

//Class to represent the player paddle
class paddle: public moving_entity {
    static sf::Texture texture;

    void process_player_input();

public:
    //Interface of the class
    paddle(float x, float y);

    // Implement the helper functions
    void move_up() noexcept override;
    void move_left() noexcept override;
    void move_right() noexcept override;
    void move_down() noexcept override;

    void apply_powerup(powerup& p);

    void update() override;
    void draw(sf::RenderWindow& window) override;
};

#endif