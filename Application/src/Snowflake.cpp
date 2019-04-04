#include <Snowflake.hpp>

float randf(float min, float max);
float Lerp(float cur, float target, float t);

/*
 * TODO: Inherit RenderObject class to implement animation
 */
Snowflake::Snowflake(Engine::Mesh* mesh, Engine::Material* material) : Engine::RenderObject(mesh, material)
{
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	fallSpeed = 1.0f;
	oscillationRadius = 1.0f;
	oscillationAmplitude = 3.1415926f / 6.0f;
	oscillationFrequency = 2.0f;
}

void Snowflake::Animate(float deltaTime)
{
	float v = GetFallSpeed();
	float a = GetOscillationAmplitude();
	float f = GetOscillationFrequency();
	float t = addTime(deltaTime);
	float r = GetOscillationRadius() * log(t);

	glm::vec3 pos = GetPosition();
	SetPosition(
		pos
		- glm::vec3(0.0f, v, 0.0f) * deltaTime
		- a * r * f * sin(f * t) * glm::vec3(cos(a * cos(f * t)), sin(a * cos(f * t)), 0.0f) * deltaTime
	);
	SetOrientation(
		glm::rotate(glm::mat4(1.0f), a * sin(f * t), glm::vec3(0.0f, 0.0f, 1.0f))
	);

	glm::vec4 color = GetColor();
	glm::vec3 org_color = color;
	float cur_alpha = color.w;
	if (t < FADE_IN_END)
		SetColor(
			glm::vec4(org_color, Lerp(cur_alpha, 0.6f, deltaTime))
		);
	else if (t < FADE_OUT_START)
		SetColor(
			glm::vec4(org_color, 0.6f)
		);
	else if (t < FADE_OUT_END)
		SetColor(
			glm::vec4(org_color, Lerp(cur_alpha, 0.0f, deltaTime))
		);
	else
	{
		SetColor(
			glm::vec4(org_color, 0.0f)
		);
		SetFlagRemove(true);
	}
}

glm::vec4 Snowflake::GetColor()
{
	return color;
}

void Snowflake::SetColor(glm::vec4 color)
{
	this->color = glm::vec4(color);
}

float Snowflake::GetFallSpeed()
{
	return fallSpeed;
}

float Snowflake::GetOscillationRadius()
{
	return oscillationRadius;
}

float Snowflake::GetOscillationAmplitude()
{
	return oscillationAmplitude;
}

float Snowflake::GetOscillationFrequency()
{
	return oscillationFrequency;
}

float Snowflake::addTime(float deltaTime)
{
	timeElapsed += deltaTime;
	return timeElapsed;
}

void Snowflake::RandomizeProperties()
{
	timeElapsed = randf(0.0f, 1.0f);
	oscillationFrequency = randf(1.0f, 3.0f);
	oscillationRadius = randf(0.2f, 2.0f);
}

void Snowflake::SetFlagRemove(bool flag)
{
	flag_remove = flag;
}

bool Snowflake::GetFlagRemove()
{
	return flag_remove;
}