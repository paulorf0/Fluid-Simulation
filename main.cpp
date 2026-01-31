#include "commands.hpp"
#include "engine/engine.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    float width = 800, height = 800;

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}), "Fluid Simulator");
    window.setFramerateLimit(60);

    Engine *eng = new Engine(10, 10, width, height);
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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::C))
            {
                commd.start_drag();
            }

            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::C))
            {
                commd.end_drag();
            }

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

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            commd.update_drag();

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