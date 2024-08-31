#include "powerup.h"

//Initialize static data
sf::Texture powerup::texture;

powerup::powerup(float x, float y, Type type, std::string name)
    : moving_entity(), type(type), name(name) {
    //Load the texture
    texture.loadFromFile("sprites/fireball.png");
    sprite.setTexture(texture);

    //Set position of the powerup
    sprite.setPosition(x, y);
    velocity = {0, constants::powerup_speed};

    //By default, operations are relative to the sprite's top lh corner
    //Make them relative to the sprite's center
    sprite.setOrigin(get_centre());
}


void powerup::move_up() noexcept {
    //This powerup doesnt move up
}

void powerup::move_down() noexcept {
    velocity.y = constants::powerup_speed;
}

void powerup::move_left() noexcept {
    //This powerup doesnt move left
}

void powerup::move_right() noexcept {
    //This powerup doesnt move right
}

//Compute the powerup's new position
void powerup::update() {
    //Move the position of the powerup, it can only ever move downwards from where it spawns
    sprite.move(velocity);
}

//Ask the window to draw the sprite for us
void powerup::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

//Returns the powerup type
powerup::Type powerup::get_type() const {
    return type;
}

std::string powerup::get_name() const {
    return name;
}