#include "paddle.h"
#include <iostream>

//Initialize static data
sf::Texture paddle::texture;

paddle::paddle(float x, float y): moving_entity() {
    //Load the texture
    texture.loadFromFile("sprites/paddle.png");
    sprite.setTexture(texture);

    sprite.setPosition(x, y);
    velocity = {constants::paddle_speed, 0.0f};

    //By default, operations are relative to the sprite's top lh corner
    //Make them relative to the sprite's centre
    sprite.setOrigin(get_centre());
}

//Compute the ball's new position
void paddle::update() {
    process_player_input();

    sprite.move(velocity);
}

void paddle::draw(sf::RenderWindow& window) {
    //Ask the window to draw the sprite for us
    auto box = get_bounding_box();
    window.draw(sprite);
}

void paddle::move_up() noexcept {}

void paddle::move_down() noexcept {}

void paddle::move_left() noexcept {
    velocity.x = -constants::paddle_speed;
}

void paddle::move_right() noexcept {
    velocity.x = constants::paddle_speed;
}

void paddle::process_player_input() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
        //This works for the lefthand side of the screen
        if(x() >= 0)
            velocity.x = -constants::paddle_speed;
        else
            velocity.x = 0;
    }
    //Move to the right
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
        //This works for the righthand side, but maybe it is ugly
        if (x() <= constants::window_width)
            velocity.x = constants::paddle_speed;
        else
            velocity.x = 0;
    }
    else {
        velocity.x = 0;
    }
}