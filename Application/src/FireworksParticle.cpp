#include "FireworksParticle.hpp"

constexpr float PI = 3.1415926f;
constexpr float PARTICLE_AVG_VELOCITY = 6.0f;
constexpr float PARTICLE_DEV_VELOCITY = 1.0f;
constexpr float PARTICLE_AVG_LIFETIME = 2.0f;
constexpr float PARTICLE_DEV_LIFETIME = 0.2f;
constexpr float PARTICLE_SIZE_MULTIPLIER = 0.15f;
constexpr float PARTICLE_MIN_YSIZE = 3.0f;
constexpr float PARTICLE_DEV_COLOR = 0.2f;
constexpr float PARTICLE_GRAVITY = 2.0f;

float randf(float min, float max);
float Lerp(float cur, float target, float t);

float len(glm::vec2 v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

FireworksParticle::FireworksParticle(Engine::Mesh *mesh, Engine::Material *material) : Engine::RenderObject(mesh, material)
{
	init_v = glm::vec2(PARTICLE_AVG_VELOCITY, 0.0f);
	SetVelocity(init_v);
	timeElapsed = 0.0f;
	lifetime = PARTICLE_AVG_LIFETIME;
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void FireworksParticle::RandomizeProperties(glm::vec3 c)
{
	float v_angle = randf(-PI, PI);
	init_v = glm::vec2(cos(v_angle), sin(v_angle)) 
		* randf(PARTICLE_AVG_VELOCITY - PARTICLE_DEV_VELOCITY, PARTICLE_AVG_VELOCITY + PARTICLE_DEV_VELOCITY);
	SetVelocity(init_v);
	lifetime = randf(PARTICLE_AVG_LIFETIME - PARTICLE_DEV_LIFETIME, PARTICLE_AVG_LIFETIME + PARTICLE_DEV_LIFETIME);
	SetColor(glm::vec4(
		randf(c.x - PARTICLE_DEV_COLOR, c.x + PARTICLE_DEV_COLOR),
		randf(c.y - PARTICLE_DEV_COLOR, c.y + PARTICLE_DEV_COLOR),
		randf(c.z - PARTICLE_DEV_COLOR, c.z + PARTICLE_DEV_COLOR),
		1.0f
	));
}


FireworksParticle::~FireworksParticle()
{
}

void FireworksParticle::Animate(float deltaTime)
{
	if (!flag_remove)
	{
		float t = addTime(deltaTime);
		float vel = len(init_v);

		if (t < lifetime)
		{
			glm::vec4 c = GetColor();
			glm::vec3 org_c = c;

			SetVelocity(init_v * (Lerp(vel, 0.0f, t / lifetime) / vel) + t * PARTICLE_GRAVITY * glm::vec2(0.0f, -1.0f));
			SetPosition(GetPosition() + glm::vec3(GetVelocity() * deltaTime, 0.0f));
			SetColor(glm::vec4(org_c, Lerp(1.0f, 0.0f, t / lifetime)));
		}
		else
		{
			SetVelocity(glm::vec2(0.0f, 0.0f));
			SetFlagRemove(true);
		}
	}
}

glm::vec4 FireworksParticle::GetColor()
{
	return color;
}

void FireworksParticle::SetColor(glm::vec4 color)
{
	this->color = glm::vec4(color);
}


float FireworksParticle::addTime(float deltaTime)
{
	timeElapsed += deltaTime;
	return timeElapsed;
}


void FireworksParticle::SetFlagRemove(bool flag)
{
	flag_remove = flag;
}

bool FireworksParticle::GetFlagRemove()
{
	return flag_remove;
}

void FireworksParticle::SetVelocity(glm::vec2 velocity)
{
	v = velocity;

	SetOrientation(glm::rotate(glm::mat4(1.0f), atan2(v.y, v.x) + 0.5f * PI, glm::vec3(0.0f, 0.0f, 1.0f)));
	SetScale(PARTICLE_SIZE_MULTIPLIER * glm::vec3(1.0f, PARTICLE_MIN_YSIZE + len(v), 1.0f));
}

glm::vec2 FireworksParticle::GetVelocity()
{
	return v;
}

void FireworksParticle::SetLifetime(float t)
{
	lifetime = t;
}

float FireworksParticle::GetLifetime()
{
	return lifetime;
}