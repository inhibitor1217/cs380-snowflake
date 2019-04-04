#include <Fractal.hpp>

#include <math.h>
#include <vector>

/*
 * Implement Fractal class to generate Koch snowflakes mesh (generate snowflake mesh using triangle list).
 * Koch snowflake: https://en.wikipedia.org/wiki/Koch_snowflake
 */

Fractal::Fractal()
{

}

Fractal::~Fractal()
{

}

/*
 * TODO: Implement koch snowflake mesh generator
 */
void Fractal::GenerateSnowflake(Engine::Mesh* mesh)
{
	mesh->AddAttribute(4);

	/* Generate initial triangle */
	std::vector<glm::vec2> vertices, prev_vertices {
		glm::vec2( 0.0f, 2*sqrt(3)/3),
		glm::vec2(-1.0f,  -sqrt(3)/3),
		glm::vec2( 1.0f,  -sqrt(3)/3)
	};
	int idx = 0, offset = 1;

	for (int i = 0; i < FRACTAL_STEPS; i++, offset *= 4)
	{
		vertices.clear();
		for (int j = 0; j < prev_vertices.size(); j++)
		{
			glm::vec2 diff = prev_vertices[(j + 1) % prev_vertices.size()] - prev_vertices[j];
			glm::vec2 diff_ortho = glm::vec2(diff.y, -diff.x);

			vertices.push_back(glm::vec2(prev_vertices[j]));
			vertices.push_back(prev_vertices[j] + (1.0f / 3) * diff);
			vertices.push_back(prev_vertices[j] + 0.5f * diff + ((float)sqrt(3) / 6.0f) * diff_ortho);
			vertices.push_back(prev_vertices[j] + (2.0f / 3) * diff);
		}
		prev_vertices = vertices;
	}

	while (offset)
	{
		for (; idx < vertices.size(); idx += offset)
		{
			if (idx > 0 && idx % (4 * offset) == 0)
				continue;
			mesh->AddVertexData(vertices[idx]);
			mesh->AddVertexData(0.0f);
			mesh->AddVertexData(1.0f);
		}
		offset /= 4;
		idx = offset;
	}

	mesh->SetNumElements(vertices.size());
	mesh->CreateMesh();
}