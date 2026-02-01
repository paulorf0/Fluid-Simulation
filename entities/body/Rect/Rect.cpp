#include "Rect.hpp"

sf::Vector2f Rect::normal(Particle *p)
{
    sf::Vector2f diff = p->get_pos() - center;
    float r = sqrt(diff.x * diff.x + diff.y * diff.y);

    if (r < 0.01f)
    {
        return sf::Vector2f(0, -1);
    }

    return diff / r;
}

sf::Vector2f Rect::get_velocity_at(sf::Vector2f pos)
{
    return sf::Vector2f();
}

bool Rect::check_collision(Particle *p)
{
    auto p_pos = p->get_pos();
    auto pos = points[0];
    float r = p->get_radius();

    float closestX = std::clamp(p_pos.x, pos.x, pos.x + width);
    float closestY = std::clamp(p_pos.y, pos.y, pos.y + height);

    float distX = p_pos.x - closestX;
    float distY = p_pos.y - closestY;

    float distSquare = (distX * distX) + (distY * distY);

    return distSquare < (r * r);
}

bool Rect::check_inside(Particle *p)
{
    return check_collision(p);
}

void Rect::clearBuffers()
{
}

void Rect::applyImpulse(sf::Vector2f pos, sf::Vector2f I)
{
}

void Rect::extractParticle(Particle *p)
{
    sf::Vector2f p_pos = p->get_pos();
    float r = p->get_radius();
    float epsilon = 0.001f; 

    float minX = points[0].x;
    float maxX = points[0].x + width;
    float minY = points[0].y;
    float maxY = points[0].y + height;

    float closestX = std::clamp(p_pos.x, minX, maxX);
    float closestY = std::clamp(p_pos.y, minY, maxY);

    float distX = p_pos.x - closestX;
    float distY = p_pos.y - closestY;
    float distSq = (distX * distX) + (distY * distY);

    if (distSq > 0.000001f)
    {
        float dist = std::sqrt(distSq);

        sf::Vector2f n = {distX / dist, distY / dist};
        p->set_pos(sf::Vector2f(closestX, closestY) + n * (r + epsilon));
    }
    else
    {
        float dLeft = std::abs(p_pos.x - minX);
        float dRight = std::abs(maxX - p_pos.x);
        float dTop = std::abs(p_pos.y - minY);
        float dBottom = std::abs(maxY - p_pos.y);

        float minD = std::min({dLeft, dRight, dTop, dBottom});

        if (minD == dTop) 
            p->set_pos({p_pos.x, minY - r - epsilon});
        else if (minD == dBottom) 
            p->set_pos({p_pos.x, maxY + r + epsilon});
        else if (minD == dLeft) 
            p->set_pos({minX - r - epsilon, p_pos.y});
        else if (minD == dRight)
            p->set_pos({maxX + r + epsilon, p_pos.y});
    }
}
