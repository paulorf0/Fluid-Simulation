#pragma once

#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <iostream>

#include "../kernel/kernel.hpp"
#include "../entities/particle/particle.hpp"
#include "../entities/body/body.hpp"

typedef struct _Spring_
{
    Particle *A;
    Particle *B;
    float L;
} Spring;

struct EdgeKey
{
    size_t id1, id2;

    EdgeKey(size_t a, size_t b)
    {
        id1 = std::min(a, b);
        id2 = std::max(a, b);
    }

    bool operator==(const EdgeKey &other) const
    {
        return id1 == other.id1 && id2 == other.id2;
    }
};

struct EdgeHash
{
    size_t operator()(const EdgeKey &k) const
    {
        return std::hash<size_t>{}(k.id1) ^ (std::hash<size_t>{}(k.id2) << 1);
    }
};

typedef struct _Cell_
{
    int x, y;
    bool operator==(const _Cell_ &other) const
    {
        return x == other.x && y == other.y;
    }
} Cell;

struct CellHasher
{
    std::size_t operator()(const Cell &c) const
    {
        std::size_t h1 = std::hash<int>{}(c.x);
        std::size_t h2 = std::hash<int>{}(c.y);
        return h1 ^ (h2 << 1);
    }
};

class Engine
{
public:
    Engine(float mass, float radius, float w, float h) : mass(mass), radius(radius), width(w), height(h)
    {
        particles_count = 0;
        bodies = std::vector<Body*>();
        max_particles = 10000;
        particles.reserve(max_particles);
    }

    void step();
    std::vector<Particle *> *get_particle(sf::Vector2i pos);
    const std::vector<Particle> &getParticles() const { return particles; }

    int add_particle(sf::Vector2i pos);
    void add_body(Body* body) { bodies.push_back(body); };

    void applyForce(sf::Vector2f pos, float radius, float strength);
    int get_particles_count() const { return particles_count; };

private:
    static constexpr float rho0 = 10.0f; // Densidade de repouso.

    static constexpr float k = 0.01f; // Rigidez da pressão (Stiffness).

    static constexpr float k_near = 0.2f; // Rigidez da densidade próxima
                                          // Controla a anti-aglomeração (anti-clustering).

    static constexpr float DT = 1.0f;
    static constexpr float h = 20.0f; // Raio de interação
                                      // Deve ser maior que o diâmetro da partícula para haver vizinhos suficientes
                                      // Aprox 1.5 a 2x o diametro da particula

    static constexpr float k_spring = 0.9f; // Rigidez da "mola"

    static constexpr float alpha = 0.6f; // Plasticidade da "mola"
                                         // Quanto maior, mais rápido o fluido "esquece" sua forma original.

    static constexpr float gamma = 0.1f; // Taxa de deformação da "mola"

    static constexpr float sigma = 0.9f; // Viscosidade Linear. Viscosidade do fluído

    static constexpr float beta = 0.0f; // Viscosidade quadrática
    static constexpr float mi = 0.4f;   // Coeficiente de Fricção. 0 = escorregadio, 1 = aderente
    static constexpr float e = 0.97f;

    static inline const sf::Vector2f GRAVITY{0.f, 0.2f};

    static constexpr float wall_restitution = 0.5;

    float mass;   // particle mass
    float radius; // particle radius

    int particles_count;
    int max_particles;
    float width;
    float height;

    std::unordered_map<Cell, std::vector<Particle *>, CellHasher> space;
    std::vector<Particle> particles;
    std::vector<Body*> bodies;
    std::unordered_map<EdgeKey, Spring, EdgeHash> springMap;

    void applyViscosity();
    void doubleDensityRelaxation();
    void applySpringDisplacements();
    void adjustSpring();
    void resolveCollisions();

    Spring *getSpring(Particle *a, Particle *b);
    void addSpring(Particle *a, Particle *b, float restLength);
    inline float dot(sf::Vector2f a, sf::Vector2f b) { return a.x * b.x + a.y * b.y; };
};