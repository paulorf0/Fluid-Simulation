#include "engine.hpp"

void Engine::step()
{
    space.clear();
    for (auto &p : particles)
    {
        int cx = (int)std::floor(p.get_pos().x / h);
        int cy = (int)std::floor(p.get_pos().y / h);
        space[{cx, cy}].push_back(&p);
    }

    for (auto &p : particles)
    {
        p.set_prev_pos(p.get_pos());
        p.set_vel(p.get_vel() + DT * GRAVITY);
    }

    applyViscosity();
    for (auto &p : particles)
        p.set_pos(p.get_pos() + DT * p.get_vel());

    adjustSpring();
    applySpringDisplacements();
    doubleDensityRelaxation();
    resolveCollisions();

    for (auto &p : particles)
    {
        sf::Vector2f vel_next = (p.get_pos() - p.get_prev_pos()) / DT;
        p.set_vel(vel_next);
    }
}

int Engine::add_particle(sf::Vector2i pos)
{
    if (particles_count >= max_particles)
        return particles_count;

    sf::Vector2f pos_float = sf::Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y));
    Particle p(particles_count, mass, radius, pos_float);
    particles.push_back(p);

    ++particles_count;
    std::cout << "Quantidade de particulas: " << particles_count << std::endl;
    return particles_count;
}

std::vector<Particle *> *Engine::get_particle(sf::Vector2i pos)
{
    int cx = (int)std::floor(pos.x / h);
    int cy = (int)std::floor(pos.y / h);

    Cell neighborKey = {cx, cy};
    auto it = space.find(neighborKey);
    if (it != space.end())
    {
        return &it->second;
    }
    return {};
}

void Engine::applyForce(sf::Vector2f pos, float radius, float strength)
{
    float rSq = radius * radius;

    for (auto &p : particles)
    {
        sf::Vector2f diff = p.get_pos() - pos;
        float distSq = diff.x * diff.x + diff.y * diff.y;

        if (distSq < rSq && distSq > 0.001f)
        {
            float dist = std::sqrt(distSq);
            sf::Vector2f dir = diff / dist;

            float factor = (1.0f - dist / radius);

            p.set_vel(p.get_vel() + dir * strength * factor);
        }
    }
}

void Engine::applyViscosity()
{
    const float h_sq = h * h;
    const float dt_sq = DT * DT;

    for (auto &pari : particles)
    {
        float rho = 0.f;
        float rho_near = 0.f;

        int cx = (int)std::floor(pari.get_pos().x / h);
        int cy = (int)std::floor(pari.get_pos().y / h);

        // Density
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                Cell neighborKey = {cx + x, cy + y};
                auto it = space.find(neighborKey);

                if (it != space.end())
                {
                    for (auto *parj : it->second)
                    {
                        if (&pari == parj)
                            continue;

                        sf::Vector2f dif = parj->get_pos() - pari.get_pos(); // i -> j
                        float r2 = dif.x * dif.x + dif.y * dif.y;

                        if (r2 < h_sq)
                        {
                            float r = sqrt(r2);
                            float q = r / h;

                            if (q < 1.f)
                            {
                                sf::Vector2f diff_vel, r_hat;
                                diff_vel = (pari.get_vel() - parj->get_vel());
                                r_hat = dif / r;

                                float u = diff_vel.x * r_hat.x + diff_vel.y + r_hat.y;
                                if (u > 0)
                                {
                                    sf::Vector2f I;
                                    float term = DT * (1 - q) * (sigma * u + beta * pow(u, 2));
                                    I = r_hat * term;
                                    pari.set_vel(pari.get_vel() - I / 2.f);
                                    parj->set_vel(parj->get_vel() + I / 2.f);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Engine::doubleDensityRelaxation()
{
    const float h_sq = h * h;
    const float dt_sq = DT * DT;

    for (auto &pari : particles)
    {
        float rho = 0.f;
        float rho_near = 0.f;

        int cx = (int)std::floor(pari.get_pos().x / h);
        int cy = (int)std::floor(pari.get_pos().y / h);

        // Density
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                Cell neighborKey = {cx + x, cy + y};
                auto it = space.find(neighborKey);

                if (it != space.end())
                {
                    for (auto *parj : it->second)
                    {
                        if (&pari == parj)
                            continue;

                        sf::Vector2f dif = parj->get_pos() - pari.get_pos(); // i -> j
                        float r2 = dif.x * dif.x + dif.y * dif.y;

                        if (r2 < h_sq)
                        {
                            float r = sqrt(r2);
                            float q = r / h;

                            if (q < 1.f)
                            {
                                float one_minus_q = 1.0f - q;
                                rho += one_minus_q * one_minus_q;
                                rho_near += one_minus_q * one_minus_q * one_minus_q;
                            }
                        }
                    }
                }
            }
        }

        float P = k * (rho - rho0);
        float P_near = k_near * rho_near;
        sf::Vector2f dx(0, 0);

        // Displacements
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                Cell neighborKey = {cx + x, cy + y};
                auto it = space.find(neighborKey);

                if (it != space.end())
                {
                    for (auto *parj : it->second)
                    {
                        if (&pari == parj)
                            continue;

                        sf::Vector2f dif = parj->get_pos() - pari.get_pos();
                        float r2 = dif.x * dif.x + dif.y * dif.y;

                        if (r2 < h_sq && r2 > 0.000001f)
                        {
                            float r = sqrt(r2);
                            float q = r / h;

                            if (q < 1.f)
                            {
                                float one_minus_q = 1.0f - q;
                                sf::Vector2f r_hat = dif / r;

                                float magnitude = dt_sq * (P * one_minus_q + P_near * (one_minus_q * one_minus_q));

                                sf::Vector2f D = r_hat * magnitude;
                                parj->set_pos(parj->get_pos() + D * 0.5f);

                                dx -= D * 0.5f;
                            }
                        }
                    }
                }
            }
        }
        pari.set_pos(pari.get_pos() + dx);
    }
}

void Engine::applySpringDisplacements()
{
    float dt2 = DT * DT;

    for (auto &item : springMap)
    {
        auto &[key, spring] = item;

        sf::Vector2f dif = spring.B->get_pos() - spring.A->get_pos();
        float r = sqrt(dif.x * dif.x + dif.y * dif.y);

        sf::Vector2f D;
        float factor = dt2 * k_spring * (1 - spring.L / h) * (spring.L - r);
        D = (dif / r) * factor;

        spring.A->set_pos(spring.A->get_pos() - D * 0.5f);
        spring.B->set_pos(spring.B->get_pos() + D * 0.5f);
    }
}

void Engine::adjustSpring()
{
    for (auto &pari : particles)
    {
        int cx = (int)std::floor(pari.get_pos().x / h);
        int cy = (int)std::floor(pari.get_pos().y / h);

        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                Cell neighborKey = {cx + x, cy + y};
                auto it = space.find(neighborKey);

                if (it != space.end())
                {
                    for (auto *parj : it->second)
                    {
                        if (&pari == parj)
                            continue;

                        sf::Vector2f dif = parj->get_pos() - pari.get_pos(); // i -> j
                        float r2 = dif.x * dif.x + dif.y * dif.y;

                        if (r2 < h * h)
                        {
                            float r = sqrt(r2);
                            float q = r / h;

                            if (q < 1.f)
                            {
                                float d = 0;
                                Spring *spring = getSpring(&pari, parj);
                                if (spring == nullptr)
                                {
                                    addSpring(&pari, parj, h);
                                    spring = getSpring(&pari, parj);
                                    d = gamma * h;
                                }
                                else
                                {
                                    d = gamma * spring->L;
                                }

                                if (r > spring->L + d)
                                {
                                    spring->L = spring->L + DT * alpha * (r - spring->L - d);
                                }
                                else if (r < spring->L - d)
                                {
                                    spring->L = spring->L + DT * alpha * (spring->L - r - d);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    std::erase_if(springMap, [](const auto &item)
                  {
            auto const& [key, spring] = item; 
            return spring.L > h; });
}
void Engine::resolveCollisions()
{
    for (auto &body : bodies)
    {
        // body.save_state(); 
        // body.integrate_prediction(DT); 
        body->clearBuffers(); 

        for (auto &p : particles)
        {
            if (body->check_inside(&p)) 
            {
                // sf::Vector2f n = body->normal(&p);
                // sf::Vector2f relative_v = p.get_vel() - body->get_velocity_at(p.get_pos());
                // float v_dot_n = dot(relative_v, n);
                // sf::Vector2f v_normal = v_dot_n * n;
                // sf::Vector2f v_tan = relative_v - v_normal;
                // sf::Vector2f I = -(1.0f + e) * v_normal + mi * v_tan;

                // body->add_force_at_pos(-I, p.get_pos()); 
            }
        }
    }

    for (auto &body : bodies)
    {
        // body->update_physics(DT); 
    }
    
    for (auto &body : bodies)
    {
        for (auto &p : particles) 
        {
            if (body->check_inside(&p)) 
            {
                sf::Vector2f n = body->normal(&p);
                sf::Vector2f relative_v = p.get_vel() - body->get_velocity_at(p.get_pos());
                float v_dot_n = dot(relative_v, n);
                
                sf::Vector2f v_normal = v_dot_n * n;
                sf::Vector2f v_tan = relative_v - v_normal;
                sf::Vector2f I = -(1.0f + e) * v_normal + mi * v_tan;

                p.set_vel(p.get_vel() + I);
                body->extractParticle(&p);
            }
        }
    }
}

Spring *Engine::getSpring(Particle *a, Particle *b)
{
    EdgeKey key(a->get_id(), b->get_id());
    auto it = springMap.find(key);
    if (it != springMap.end())
    {
        return &(it->second);
    }
    return nullptr;
}

void Engine::addSpring(Particle *a, Particle *b, float restLength)
{
    EdgeKey key(a->get_id(), b->get_id());
    springMap[key] = {a, b, restLength};
}
