#pragma once

#include "../body.hpp"


class Circle : public Body {
public:
    Circle(sf::Vector2f pos, float r) :
    Body(pos), radius(r) {}

    sf::Vector2f normal(Particle* p);
    sf::Vector2f get_velocity_at(sf::Vector2f pos);

    int get_radius();
    bool check_collision(Particle* p);
    bool check_inside(Particle* p); 
    void clearBuffers();
    void applyImpulse(sf::Vector2f pos, sf::Vector2f I);
    void extractParticle(Particle* p);

private:
    int radius;
};