#include "commands.hpp"
#include <limits>

void Commands::start_drag()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition();

    std::vector<Particle *> *vec = eng->get_particle(mousePos);

    if (!vec)
        return;

    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    float minDist = std::numeric_limits<float>::max();
    Particle *closest = nullptr;

    for (auto &p : *vec)
    {
        sf::Vector2f diff = p->get_pos() - mousePosF;
        float distSq = diff.x * diff.x + diff.y * diff.y;

        if (distSq < interactionRadius * interactionRadius && distSq < minDist)
        {
            minDist = distSq;
            closest = p;
        }
    }

    if (closest)
    {
        selectedParticle = closest;
        isDragging = true;
    }
}

void Commands::update_drag()
{
    if (!isDragging || !selectedParticle)
        return;

    sf::Vector2i mousePos = sf::Mouse::getPosition();
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    sf::Vector2f direction = mousePosF - selectedParticle->get_pos();

    selectedParticle->set_vel(selectedParticle->get_vel() + direction * dragStiffness * 0.1f);
    selectedParticle->set_vel(selectedParticle->get_vel() * 0.9f);
}

void Commands::end_drag()
{
    isDragging = false;
    selectedParticle = nullptr;
}

void Commands::add_fluid(sf::Vector2i pos)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    float radius = 60.0f;
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
    fluid_output += 100;
}

void Commands::decrease_fluid_output()
{
    if (fluid_output <= 1)
        return;
    fluid_output -= 100;
}
