#include "background.h"

//Initialize static data
sf::Texture background::texture;

background::background(float x, float y): entity() {
    //Load the texture
    texture.loadFromFile("sprites/background.jpg");
    sprite.setTexture(texture);

    //Set the initial position and velocity of the background
    //User (x, y) for the initial position of the background
    sprite.setPosition(x, y);
}

//Compute the backgrounds new position
void background::update() {
    //Nothing to do here
}

void background::draw(sf::RenderWindow& window) {
    //Nothing to do here
    window.draw(sprite);
}