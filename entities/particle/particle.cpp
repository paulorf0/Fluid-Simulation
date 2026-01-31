#include "particle.hpp"

void Particle::apply_force(sf::Vector2f force){
    this->force += force;
}

void Particle::set_force(sf::Vector2f force) {
    this->force = force;
}

void Particle::set_pressure(float pressure)
{
    p = pressure;
}

void Particle::set_pressure_near(float pressure_near)
{
    this->p_near = pressure_near;
}

void Particle::set_density(float density)
{
    rho = density;
}

void Particle::set_prev_pos(sf::Vector2f prev_pos)
{
    this->prev_pos = prev_pos;
}

void Particle::set_vel(sf::Vector2f vel)
{
    this->vel = vel;
}

void Particle::set_pos(sf::Vector2f pos){
    // this->prev_pos = this->pos;
    this->pos = pos;
}