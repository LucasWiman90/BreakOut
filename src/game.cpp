#include <algorithm>

#include "game.h"
#include "interactions.h"

//Function to scan all entities and clean up destroyed ones
void entity_manager::refresh() {
    //We must clean up the alias pointers first, to avoid dangling pointers
    //Simply remove from their vector
    for(auto& [type, alias_vector] : grouped_entities) {

        alias_vector.erase(remove_if(begin(alias_vector), end(alias_vector),
                [](const auto& p) { return p->is_destroyed(); }
                ),
        end(alias_vector));
    }

    //Now objects can be destroyed since there are no aliases to them
    all_entities.erase(remove_if(begin(all_entities), end(all_entities),
            [](const auto& p) { return p->is_destroyed(); }
            ),
    end(all_entities));
}

//Function to destroy all entities
void entity_manager::clear() {
    //Again, we must clean up the alias pointers first
    grouped_entities.clear();
    all_entities.clear();
}

//Function to update all entities
void entity_manager::update() {
    for (auto& e : all_entities)
        e->update();
}

// Function to update make all the entities draw themselves
void entity_manager::draw(sf::RenderWindow& window) {
	for (auto& e : all_entities)
	    e->draw(window);
}

game::game() {
    //Limit the framerate
    game_window.setFramerateLimit(60);

    //Load a font from file
    verdana.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/verdana.ttf");
    
    text_state.setFont(verdana);
    text_state.setPosition(constants::window_width/2.0f-100.0f, constants::window_height/2.0f-100.0f);
    text_state.setCharacterSize(35);
    text_state.setFillColor(sf::Color::White);
    text_state.setString("Paused");

    text_lives.setFont(verdana);
    text_lives.setPosition(constants::window_width/2.0f-65.0f, constants::window_height/2.0f-50.0f);
    text_lives.setCharacterSize(35);
    text_lives.setFillColor(sf::Color::White);
    text_lives.setString("Lives: " + std::to_string(lives));
}

//(Re)initialize the game and reset the state of the ball and paddle
void game::reset() {

    lives = constants::player_lives;
    state = game_state::paused;

    // Destroy all the entities and re-create them
	manager.clear();
    manager.create<background>(0.0f, 0.0f);
    manager.create<ball>(constants::window_width/2.0f, constants::window_height/2.0f);

    // Test create a powerup
    manager.create<powerup>(constants::window_width/4.0f, constants::window_height/4.0f, powerup::Type::Ball, "fireball");

	manager.create<paddle>(constants::window_width/2.0f, constants::window_height - constants::paddle_height);

    //Repopulate the bricks
    for (int i = 0; i < constants::brick_columns; ++i) {
        for (int j = 0; j < constants::brick_rows; ++j) {
            ///Calculate bricks position
            float x = constants::brick_offset + (i + 1) * constants::brick_width;
            float y = (j + 1) * constants::brick_height;

            //Create the brick object
            manager.create<brick>(x, y);
        }
    }

    //Limit the framerate
    game_window.setFramerateLimit(60);
}

//(Re)start the game
void game::run() {
    //Was the pause key pressed in the last frame?
    bool pause_key_active{false};

    while (game_window.isOpen()) {
        //Clear the screen
        game_window.clear(sf::Color::Black);

        handle_events();
        handle_input(pause_key_active);

        //If the game is not running, the entities are not updated
        //They are redrawn only if the game is paused.
        if (state == game_state::paused) {
            manager.draw(game_window);
        }

        //Choose the correct text for the state of the game
        if (state == game_state::paused ||state != game_state::running) {
            handle_non_running_state();  
        } else {
            handle_running_state();
        }

        //Update life string to ensure it is correct
        text_lives.setString("Lives: " + std::to_string(lives));
        game_window.display();
    }
}

//Check for any events since the last loop iteration
//If the user pressed "Escape", or clicked on "close", we close the window
//This will terminate the program
void game::handle_events() {
    sf::Event event;
    while (game_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            game_window.close();
        }
    }
}

//Check for input, need a reference to the pause_key_active bool
//Is this good? Might want to just make pause_key_active a member variable of game instead
//then we wont need this as an in_parameter
void game::handle_input(bool &pause_key_active) {
    //Escape, we jump out of the loop and terminate the program
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        game_window.close();
    }

    //If it was not paused on the last iteration, set the pause bool
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && state != game_state::game_over) {
        if (!pause_key_active) {
            state = (state == game_state::paused) ? game_state::running : game_state::paused;
        }
        pause_key_active = true;
    } else {
        pause_key_active = false;
    }

    //If R is pressed we reset the game
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        reset();
    }
}

//If the game is not running, the entities are not updated
//They are redrawn only if the game is paused.
void game::handle_non_running_state() {
    if (state == game_state::paused) {
        manager.draw(game_window);
    }

    switch(state) {
        case game_state::paused:
            text_state.setString("   Paused   ");
            break;
        case game_state::game_over:
            text_state.setString("   Game Over!   ");
            break;
        case game_state::player_wins:
            text_state.setString("Player Wins!");
            break;
        default:
            break;
    }

    game_window.draw(text_state);
    game_window.draw(text_lives);
}

void game::handle_running_state() 
{
    //If there are no remaining balls on the screen
    if (manager.get_all<ball>().empty()) {
        manager.create<ball>(constants::window_width / 2.0f, constants::window_height / 2.0f);
        --lives;
        state = game_state::paused;
    }

    if (manager.get_all<brick>().empty()) {
        state = game_state::player_wins;
    }

    if (lives <= 0) {
        state = game_state::game_over;
    }

    //Calculate the updated graphics
    manager.update();
    handle_entity_collisions();
    manager.refresh();
    manager.draw(game_window);
}

void game::handle_entity_collisions() {
    //For every ball, call a function which
    //  For every brick, call a function which
    //    Calls handle collission with the ball and the brick as arguments
    manager.apply_all<ball>([this](auto &the_ball) {
        manager.apply_all<brick>([&the_ball](auto &the_brick) {
            handle_collision(the_ball, the_brick);
        });
    });

    //Paddle & ball interaction
    manager.apply_all<ball>([this](auto &the_ball) {
        manager.apply_all<paddle>([&the_ball](auto &the_paddle) {
            handle_collision(the_ball, the_paddle);
        });
    });

    //For every powerup, call a function which
    //  For every paddle, call a function which
    //    Calls handle collission with the paddle and powerup as arguments
    manager.apply_all<powerup>([this](auto& powerup) {
        // Apply a function to all paddle entities
        manager.apply_all<paddle>([&powerup, this](auto& the_paddle) {
            // Retrieve all ball entities
            manager.apply_all<ball>([&powerup, &the_paddle](auto& the_ball) {
                // Call handle_powerups with powerup, paddle, and ball
                handle_powerups(powerup, the_paddle, the_ball);
            });
        });
    });
}