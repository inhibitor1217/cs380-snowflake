#pragma once

#include <glm/glm.hpp>
#include <RenderObject.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

constexpr float AVERAGE_LIFE = 4.0f;
constexpr float DEVIATION_LIFE = 1.0f;

class FireworksParticle: public Engine::RenderObject
{
private:
	glm::vec4 color;
	glm::vec2 init_v;
	glm::vec2 v;
	float lifetime;
	float timeElapsed;
	bool flag_remove = false;

public:
	FireworksParticle(Engine::Mesh *mesh, Engine::Material *material);
	~FireworksParticle();
	void RandomizeProperties(glm::vec3 c);
	void Animate(float deltaTime);
	void SetColor(glm::vec4 color);
	glm::vec4 GetColor();
	float addTime(float deltaTime);
	bool GetFlagRemove();
	void SetFlagRemove(bool flag);
	glm::vec2 GetVelocity();
	void SetVelocity(glm::vec2 velocity);
	float GetLifetime();
	void SetLifetime(float t);

};

