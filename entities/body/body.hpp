#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

#include "../particle/particle.hpp"


class Body {
public:
    Body() {}
    virtual ~Body() {} 

    virtual sf::Vector2f normal(Particle* p) = 0;
    virtual sf::Vector2f get_velocity_at(sf::Vector2f pos) = 0;
    virtual void applyImpulse(sf::Vector2f pos, sf::Vector2f I) = 0;
    virtual bool check_collision(Particle* p) = 0; 
    virtual bool check_inside(Particle* p) = 0; 
    virtual void clearBuffers() = 0;
    virtual void extractParticle(Particle* p) = 0;
protected:
};