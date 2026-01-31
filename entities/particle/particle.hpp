#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

class Particle
{
public:
    Particle(int id, float mass, float radius, sf::Vector2f pos) : id(id), mass(mass), radius(radius), pos(pos)
    {
        this->rho = 0;
        this->rho_near = 0;
        this->p = 0;
        this->p_near = 0;

        this->vel = {0, 0};
        this->force = {0, 0};
        this->prev_pos = pos; 
    }

    void apply_force(sf::Vector2f force);
    int get_id() const { return id; };
    float get_radius() const { return radius; };
    float get_density() const { return rho; };
    float get_density_near() const { return rho_near; };
    sf::Vector2f get_pos() const { return pos; };
    sf::Vector2f get_prev_pos() const { return prev_pos; };
    sf::Vector2f get_vel() const { return vel; };

    void set_vel(sf::Vector2f vel);
    void set_pos(sf::Vector2f pos);
    void set_force(sf::Vector2f force);
    void set_pressure(float pressure);
    void set_pressure_near(float pressure_near);
    void set_density(float density);
    void set_prev_pos(sf::Vector2f prev_pos);

private:
    sf::Vector2f vel;
    sf::Vector2f pos;
    sf::Vector2f force;
    sf::Vector2f prev_pos;

    float mass;
    float rho;      // Density
    float rho_near; // Density
    float p;        // Pressure
    float p_near;
    float radius; // Particle Radius
    int id;
};
