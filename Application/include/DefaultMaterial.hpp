#pragma once

#include <Material.hpp>

class DefaultMaterial : public Engine::Material
{
public:
    void CreateMaterial();
    void UpdateColor(glm::vec4 color);
};