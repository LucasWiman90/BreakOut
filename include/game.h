#ifndef GAME_H
#define GAME_H

#include <memory>
#include <typeinfo>
#include <vector>

#include "background.h"
#include "ball.h"
#include "brick.h"
#include "constants.h"
#include "entity.h"
#include "powerup.h"
#include "paddle.h"

// A class to manage the entities in the game
// It stores the entities in a vector of std::unique_ptr
// The manager can create an entity object and add it to the vector

// Example: create a ball object at position (x, y) on the screen
// manager.create<ball>(x, y);

// When an entity is destroyed, the game will mark it as destroyed, but not delete it.
// Instead, the manager will scan all entities and clean up the destroyed ones

// The manager can also locate all the objects with a given entity type
// These are stored as alias pointers (non-owning) to the allocated objects
// This is useful for performing an operation on e.g. all bricks

// Use aliases to simplify the code
using entity_vector = std::vector<std::unique_ptr<entity>>;
using entity_alias_vector = std::vector<entity *>;

class entity_manager {
    // A vector to store all the entities in the game (all brick objects, background, ball, paddle)
    entity_vector all_entities;

    // A map to store the entities grouped by type
	// Can be iterated over to get a vector of all brick objects, then a vector of all ball objects, etc
	// The vector will contain non-owning pointers to the objects
	// Do not delete these pointers!
	// Do not use them after the objects they point to have been destroyed!
    std::map<size_t, entity_alias_vector> grouped_entities;
public:
    //Function to create an entity object of type T using args as the constructor arguments
    //We use variadic template to pass any number of arguments
    //Perfect forwarding to avoid unnecessary copying of the arguments
    template <typename T, typename... Args>
    T& create(Args&&... args) {
        // Check that the type parameter is derived from the entity base class
        static_assert(std::is_base_of<entity, T>::value,
        R"("T" type parameter in create() must be derived from "entity")");

        //Create a unique_ptr to the entity
        //Forward the arguments to the entity's constructor
        auto ptr{std::make_unique<T>(std::forward<Args>(args)...)};

        //Make an alias pointer to the allocated memory
        //This will be stored in the entity_type_vector
        auto ptr_alias = ptr.get();

        //Get the hash code for the entity object's type
        auto hash = typeid(T).hash_code();

        //Insert the alias pointer into the map
        grouped_entities[hash].emplace_back(ptr_alias);

        //Insert the object's pointer into the entities vector
        all_entities.emplace_back(std::move(ptr));

        //Return the new object
        return *ptr_alias;
    }

    //Function to scan all entities and clean up the destroyed ones
    void refresh();

    //Function to destroy all entities
    void clear();

    //Function to retrieve all the objects of a given type
    template <typename T>
    auto& get_all() {
        return grouped_entities[typeid(T).hash_code()];
    }

    //Apply a function to all entities of a given type
    template <typename T, typename Func>
    void apply_all(const Func& func) {
        auto& entity_group{get_all<T>()};

        for (auto ptr : entity_group)
            func(*dynamic_cast<T*>(ptr));
    }

    //Function to update all the entities
    void update();

    //Function to make all the entities draw themselves
    void draw(sf::RenderWindow& window);
};


class game {
    // Enum with allowed values for the game's state
	enum class game_state { paused, game_over, running, player_wins };

    //Create the game's window using an object of the class RenderWindow
    //The constructor takes an SFML 2D vector with the window dimensions
    //and an std::string with the window title
    //the SFML code is in the sf namespace
    sf::RenderWindow game_window{{constants::window_width, constants::window_height},
        "Breakout Game" };

    //Instead of embedding every entity in the game class, use an entity_manager
    entity_manager manager;

    //Use SFML text and font classes to communicate with the player
    sf::Font verdana;
    sf::Text text_state, text_lives, debug_text;

    //Member to store the current state of the game
    game_state state{game_state::running};

    int lives{constants::player_lives};

    // Power-up handling
    sf::Clock powerup_clock;         // Clock to track power-up time
    float powerup_duration{5.0f};    // Example: 5 seconds per power-up
    bool powerup_active{false};      // Is a power-up active?
    powerup::Type active_powerup_type{powerup::Type::None};

    void handle_events();
    void handle_input(bool &pause_key_active);
    void handle_non_running_state();
    void handle_running_state();
    void handle_entity_collisions();

    // Function to activate the power-up and start the timer
    void activate_powerup(powerup& p, paddle& paddle, ball& ball);

    // Function to deactivate the current power-up when the timer expires
    void deactivate_powerup(paddle& paddle, ball& ball);

public:
    game();

    //Reinitialize the game
    void reset();

    //Game loop
    void run();
};

#endif