#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Mesh.hpp>

constexpr auto FRACTAL_STEPS = 3;
constexpr auto FRACTAL_ALPHA_MULTIPLIER = 0.8f;

/*
 * Implement Fractal class to generate Koch snowflakes mesh (generate snowflake mesh using triangle list).
 * Koch snowflake: https://en.wikipedia.org/wiki/Koch_snowflake
 */
class Fractal
{
private:
public:
    Fractal();
    ~Fractal();
    void GenerateSnowflake(Engine::Mesh* mesh);
};