#include "commands.hpp"
#include "engine/engine.hpp"
#include "entities/body/Wall/Wall.hpp"
#include "entities/body/Circle/Circle.hpp"
#include "entities/body/Rect/Rect.hpp"

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

    Circle *circle = new Circle(sf::Vector2f(width / 2, height / 2), 100);
    Rect *rect = new Rect(sf::Vector2f(width / 2, height - 40), 40, 150);

    Engine *eng = new Engine(mass, radius, width, height);

    eng->add_body(up);
    eng->add_body(down);
    eng->add_body(left);
    eng->add_body(right);
    eng->add_body(circle);
    eng->add_body(rect);

    Commands commd(eng);

    sf::CircleShape pShape;
    pShape.setFillColor(sf::Color::Cyan);

    sf::CircleShape circleShape;
    circleShape.setFillColor(sf::Color::White);

    sf::RectangleShape rectShape;
    rectShape.setFillColor(sf::Color::White);

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                commd.increase_force();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                commd.decrease_force();
            }
        }

        commd.interact(window);
        eng->step();
        window.clear(sf::Color::Black);

        const auto &particles = eng->getParticles();
        for (const auto &p : particles)
        {
            float r = p.get_radius();

            pShape.setRadius(r);
            pShape.setOrigin(r, r);

            pShape.setPosition(p.get_pos());
            window.draw(pShape);
        }
        float r_obs = circle->get_radius();

        circleShape.setRadius(r_obs);
        circleShape.setOrigin(r_obs, r_obs);
        circleShape.setPosition(circle->get_center());
        window.draw(circleShape);

        dimension dim = rect->get_dimension();
        rectShape.setSize(sf::Vector2f(dim.w, dim.h));
        rectShape.setOrigin(dim.w / 2.0f, dim.h / 2.0f); 
        rectShape.setPosition(rect->get_center());
        window.draw(rectShape);

        window.display();
    }

    delete eng;
    return 0;
}