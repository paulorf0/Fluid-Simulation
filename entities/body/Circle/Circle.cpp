#include "Circle.hpp"

bool Circle::check_collision(Particle *p)
{
    sf::Vector2f p_pos = p->get_pos();
    sf::Vector2f c_pos = center;

    sf::Vector2f diff = p_pos - c_pos;

    float distanceSq = (diff.x * diff.x) + (diff.y * diff.y);
    float radiusSq = this->radius * this->radius;

    if (distanceSq <= radiusSq)
    {
        return true;
    }

    return false;
}

bool Circle::check_inside(Particle *p)
{
    sf::Vector2f p_pos = p->get_pos();
    sf::Vector2f c_pos = center;

    sf::Vector2f diff = p_pos - c_pos;

    float distanceSq = (diff.x * diff.x) + (diff.y * diff.y);
    float radiusSq = this->radius * this->radius;

    if (distanceSq <= radiusSq)
    {
        return true;
    }

    return false;
}

sf::Vector2f Circle::normal(Particle *p)
{
    sf::Vector2f dif = p->get_pos() - center;
    float r2 = dif.x * dif.x + dif.y * dif.y;

    if (r2 < 0.0001f)
        return sf::Vector2f(0, -1);
    float r = sqrt(r2);
    return dif / r;
}

sf::Vector2f Circle::get_velocity_at(sf::Vector2f pos)
{
    return sf::Vector2f(0, 0);
}

void Circle::clearBuffers()
{
}

void Circle::applyImpulse(sf::Vector2f pos, sf::Vector2f I)
{
}

void Circle::extractParticle(Particle *p)
{
    if (check_collision(p))
    {
        sf::Vector2f dif = p->get_pos() - center;
        float r2 = dif.x * dif.x + dif.y * dif.y;

        if (r2 < 0.0001f)
        {
            p->set_pos(center + sf::Vector2f(0, -(radius + p->get_radius())));
            return;
        }

        float r = sqrt(r2);
        sf::Vector2f n = dif / r;

        float target_dist = radius + p->get_radius();

        sf::Vector2f offset = n * target_dist;
        p->set_pos(center + offset);
    }
}