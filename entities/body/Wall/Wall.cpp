#include "Wall.hpp"

sf::Vector2f Wall::normal(Particle *p)
{
    if (dir == LEFT)
        return sf::Vector2f(1.f, 0.f);
    if (dir == RIGHT)
        return sf::Vector2f(-1.f, 0.f);
    if (dir == UP)
        return sf::Vector2f(0.f, 1.f);
    if (dir == DOWN)
        return sf::Vector2f(0.f, -1.f);

    return sf::Vector2f(0.f, 0.f);
}

sf::Vector2f Wall::get_velocity_at(sf::Vector2f pos)
{
    return sf::Vector2f();
}

bool Wall::check_collision(Particle *p)
{
    auto [x, y] = p->get_pos();
    float r = p->get_radius();

    if (dir == LEFT && x - r < limit_coord)
        return true;

    if (dir == RIGHT && x + r > limit_coord)
        return true;

    if (dir == UP && y - r < limit_coord)
        return true;

    if (dir == DOWN && y + r > limit_coord)
        return true;

    return false;
}

bool Wall::check_inside(Particle *p)
{
    return check_collision(p);
}

void Wall::clearBuffers()
{
}

void Wall::applyImpulse(sf::Vector2f pos, sf::Vector2f I)
{
}

void Wall::extractParticle(Particle *p)
{
    auto [x, y] = p->get_pos();
    float r = p->get_radius();
    float epsilon = 0.1f;

    sf::Vector2f new_pos = p->get_pos();
    if (dir == UP)
        new_pos.y = limit_coord + r + epsilon;
    else if (dir == DOWN)
        new_pos.y = limit_coord - r - epsilon;
    else if (dir == LEFT)
        new_pos.x = limit_coord + r + epsilon;
    else if (dir == RIGHT)
        new_pos.x = limit_coord - r - epsilon;

    p->set_pos(new_pos);
}