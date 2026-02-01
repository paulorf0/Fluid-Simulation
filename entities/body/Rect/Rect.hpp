#pragma once

#include "../body.hpp"

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

typedef struct _dimension_
{
    int w;
    int h;
} dimension;

class Rect : public Body
{
public:
    Rect(sf::Vector2f center, int h, int w) : center(center), width(w), height(h)
    {
        points.reserve(4);
        get_points();
    }

    sf::Vector2f normal(Particle *p);
    sf::Vector2f get_velocity_at(sf::Vector2f pos);

    dimension get_dimension() const { return {width, height}; };
    sf::Vector2f get_center() const { return center; };

    bool check_collision(Particle *p);
    bool check_inside(Particle *p);
    void clearBuffers();
    void applyImpulse(sf::Vector2f pos, sf::Vector2f I);
    void extractParticle(Particle *p);

private:
    int height;
    int width;
    sf::Vector2f center;
    std::vector<sf::Vector2f> points;

    inline void get_points()
    {
        sf::Vector2f p1 = {center.x - width / 2, center.y - height / 2};
        sf::Vector2f p2 = {center.x + width / 2, center.y - height / 2};
        sf::Vector2f p3 = {center.x - width / 2, center.y + height / 2};
        sf::Vector2f p4 = {center.x + width / 2, center.y + height / 2};

        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
    }

    inline float max(float a, float b) { return a > b ? a : b; };
    inline float min(float a, float b) { return a < b ? a : b; };
};

/*
    p1    p2
    #######
    #     #
    #  c  #
    #     #  |
    #######  | dimension / 2
  p3       p4
    ---
    dimension/2

    points = (p1, p2, p3, p4)
    p1 = (c1.x - width / 2, c1.y - height / 2)
    p2 = (c1.x + width / 2, c1.y - height / 2)
    p3 = (c1.x - width / 2, c1.y + height / 2)
    p4 = (c1.x + width / 2, c1.y + height / 2)
*/