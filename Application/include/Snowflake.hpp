#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <RenderObject.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

/*
 * TODO: Inherit RenderObject class to implement animation
 */

constexpr float FADE_IN_END = 3.0f;
constexpr float FADE_OUT_START = 14.0f;
constexpr float FADE_OUT_END = 18.0f;

class Snowflake: public Engine::RenderObject
{
private:
	glm::vec4 color;
	float fallSpeed;
	float oscillationRadius;
	float oscillationAmplitude;
	float oscillationFrequency;
	float timeElapsed = 0.0f;
	bool flag_remove = false;

public:
    Snowflake(Engine::Mesh* mesh, Engine::Material* material);
	void Animate(float deltaTime);
	void SetColor(glm::vec4 color);
	glm::vec4 GetColor();
	float GetFallSpeed();
	float GetOscillationRadius();
	float GetOscillationAmplitude();
	float GetOscillationFrequency();
	float addTime(float deltaTime);
	void RandomizeProperties();
	bool GetFlagRemove();
	void SetFlagRemove(bool flag);
};