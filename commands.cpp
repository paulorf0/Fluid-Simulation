#include "commands.hpp"
#include <limits>

void Commands::interact(sf::RenderWindow &window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    float radius = 30.0f;
    float strength = force;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        eng->applyForce(mousePosF, radius, strength);
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        eng->applyForce(mousePosF, radius, -strength);
    }
}

void Commands::add_fluid(sf::Vector2i pos)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    float radius = 80.0f;
    std::uniform_real_distribution<float> dist(-radius, radius);

    for (int i = 0; i < fluid_output; i++)
    {
        sf::Vector2i random_pos = pos;
        random_pos.x += static_cast<int>(dist(gen));
        random_pos.y += static_cast<int>(dist(gen));

        eng->add_particle(random_pos);
    }
}

void Commands::increase_fluid_output()
{
    fluid_output += increase;
}

void Commands::decrease_fluid_output()
{
    if (fluid_output <= 1)
        return;
    fluid_output -= increase;
}

void Commands::increase_force()
{
    force += 1;
}

void Commands::decrease_force()
{
    if (force == 1)
        return;
    force -= 1;
}
