#include <Animation.hpp>

Animation::Animation()
{
    _snowflakes = std::vector<Snowflake*>();
}

Animation::~Animation()
{
    _snowflakes.clear();
}

void Animation::AddSnowflake(Snowflake* snowflake)
{
    _snowflakes.push_back(snowflake);
}

void Animation::RemoveSnowflake(Snowflake *snowflake)
{
	std::vector<Snowflake *>::iterator it;

	it = find(_snowflakes.begin(), _snowflakes.end(), snowflake);
	if (it != _snowflakes.end())
	{
		_snowflakes.erase(it);
	}
}

void Animation::Animate(float deltaTime) 
{
	for (auto snowflake : _snowflakes)
	{
		snowflake->Animate(deltaTime);
	}
}