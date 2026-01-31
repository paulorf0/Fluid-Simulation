#include "commands.hpp"
#include "engine/engine.hpp"
#include "entities/body/Wall/Wall.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    float width = 800, height = 800;

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}), "Fluid Simulator");
    window.setFramerateLimit(60);

    float mass = 10;
    float radius = 10;

    Wall *up = new Wall(0, UP, width, height);
    Wall *down = new Wall(height, DOWN, width, height);
    Wall *left = new Wall(0, LEFT, width, height);
    Wall *right = new Wall(width, RIGHT, width, height);

    Engine *eng = new Engine(mass, radius, width, height);

    eng->add_body(up);
    eng->add_body(down);
    eng->add_body(left);
    eng->add_body(right);

    Commands commd(eng);

    sf::CircleShape pShape;
    pShape.setFillColor(sf::Color::Cyan);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::G))
            {
                auto mouse_pos = sf::Mouse::getPosition(window);
                commd.add_fluid(mouse_pos);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                commd.increase_fluid_output();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                commd.decrease_fluid_output();
            }
        }

        commd.interact(window);
        eng->step();
        window.clear(sf::Color::Black);

        sf::VertexArray particleSystem(sf::Points, 0);

        const auto &particles = eng->getParticles();
        for (const auto &p : particles)
        {
            float r = p.get_radius();

            pShape.setRadius(r);
            pShape.setOrigin(r, r);

            pShape.setPosition(p.get_pos());
            window.draw(pShape);
        }
        window.display();
    }

    delete eng;
    return 0;
}