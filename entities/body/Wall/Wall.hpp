#pragma once
#include "../body.hpp"
enum direction
{
    LEFT,
    RIGHT,
    DOWN,
    UP
};

class Wall : public Body
{
public:
    Wall(float limit_coord, enum direction dir, float w, float h) : limit_coord(limit_coord), dir(dir), width(w), height(h) {}

    sf::Vector2f normal(Particle *p);
    sf::Vector2f get_velocity_at(sf::Vector2f pos);

    bool check_collision(Particle *p);
    bool check_inside(Particle *p);
    void clearBuffers();
    void applyImpulse(sf::Vector2f pos, sf::Vector2f I);
    void extractParticle(Particle *p);

private:
    direction dir;
    float limit_coord;

    float width, height;
};