#pragma once

#include "engine/engine.hpp"

#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>

class Commands
{
public:
    Commands(Engine *eng) : initial_pos(sf::Mouse::getPosition()), eng(eng) {};

    int get_force() const { return force; };
    int get_fluid_output() const { return fluid_output; };

    void interact(sf::RenderWindow &window);

    void add_fluid(sf::Vector2i pos);
    void increase_fluid_output();
    void decrease_fluid_output();
    void increase_force();
    void decrease_force();

private:
    float dt = 0;

    Particle *selectedParticle = nullptr; // Ponteiro para a partícula sendo arrastada
    bool isDragging = false;
    float interactionRadius = 5.0f; // Raio para detectar o clique
    float dragStiffness = 5.0f;     // Força da mola do mouse

    sf::Vector2f initial_pos;
    Engine *eng;

    int fluid_output = 1;
    int increase = 20;
    int force = 5;
};