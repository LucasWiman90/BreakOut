#include "brick.h"

//Initialize static data
sf::Texture brick::texture;

const sf::Color brick_color_hit1{0, 255, 0, 80};  //Dull green
const sf::Color brick_color_hit2{0, 255, 0, 170}; //Medium green
const sf::Color brick_color_hit3{0, 255, 0, 255}; //Bright green

void brick::set_strength(int s) noexcept { strength = s; }
void brick::weaken() noexcept { --strength; }
bool brick::is_too_weak() const noexcept { return strength <= 0; }

brick::brick(float x, float y): entity() {
    //Load the texture
    texture.loadFromFile("sprites/brick01.png");
    sprite.setTexture(texture);

    //Set position of brick
    sprite.setPosition(x, y);

    //By default, operations are relative to the sprite's top lh corner
    //Make them relative to the sprite's center
    sprite.setOrigin(get_centre());
}

//Compute bricks new state (bricks typically dont move...)
void brick::update() {
    if (strength == 1)
        sprite.setColor(brick_color_hit1);
    else if (strength == 2)
        sprite.setColor(brick_color_hit2);
    else if (strength == 3)
        sprite.setColor(brick_color_hit3);
}

void brick::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}