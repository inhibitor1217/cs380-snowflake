// STL
#include <iostream>

#include <stdlib.h>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>

// include application headers
#include <Fractal.hpp>
#include <DefaultMaterial.hpp>
#include <PerVertexColorMaterial.hpp>
#include <Snowflake.hpp>
#include <FireworksParticle.hpp>
#include <Animation.hpp>

constexpr float PI = 3.1415926f;
constexpr float SNOWFLAKE_RATE = 2.0f;
constexpr float FIREWORK_RATE = 1.0f;
constexpr float BACKGROUND_SIZE = 20.0f;
constexpr float BACKGROUND_Z = -40.0f;
constexpr float FIREWORK_MARGIN = 4.0f;
constexpr float FIREWORK_Y_MARGIN = 8.0f;
constexpr float FIREWORK_Z = -20.0f;
constexpr float SKYLINE_SIZE = 9.0f;
constexpr float SKYLINE_Z = -30.0f;
constexpr float SKYLINE_BUILDING_AVG_WIDTH = SKYLINE_SIZE / 10.0f;
constexpr int CIRCLE_RESOLUTION = 16;

float randf(float min, float max)
{
	return min + (max - min) * (float)rand() / (float)RAND_MAX;
}

float Lerp(float cur, float target, float t)
{
	if (t < 0)
		return cur;
	if (t > 1)
		return target;
	return cur * (1 - t) + target * t;
}


glm::vec3 genColor(unsigned r, unsigned g, unsigned b)
{
	return glm::vec3(
		(float)r / (float)256,
		(float)g / (float)256,
		(float)b / (float)256
	);
}

// TODO: Implement gradient rectangle mesh generator for background
void GenerateGradientRectangle(Engine::Mesh* mesh)
{
    // Implement skybox-like rectangle very far away...
	mesh->AddAttribute(4);
	mesh->AddAttribute(4);

	std::vector<glm::vec2> pos{
		glm::vec2(-4 * BACKGROUND_SIZE,  3 * BACKGROUND_SIZE),
		glm::vec2(-4 * BACKGROUND_SIZE, -3 * BACKGROUND_SIZE),
		glm::vec2( 4 * BACKGROUND_SIZE, -3 * BACKGROUND_SIZE),
		glm::vec2(-4 * BACKGROUND_SIZE,  3 * BACKGROUND_SIZE),
		glm::vec2( 4 * BACKGROUND_SIZE, -3 * BACKGROUND_SIZE),
		glm::vec2( 4 * BACKGROUND_SIZE,  3 * BACKGROUND_SIZE)
	};
	std::vector<glm::vec3> color{
		genColor(0x09, 0x14, 0x22),
		genColor(0x15, 0x1A, 0x53),
		genColor(0x33, 0x35, 0x63),
		genColor(0x09, 0x14, 0x22),
		genColor(0x33, 0x35, 0x63),
		genColor(0x2A, 0x10, 0x2C),
	};

	for (int i = 0; i < 6; i++)
	{
		mesh->AddVertexData(pos[i]);
		mesh->AddVertexData(BACKGROUND_Z);
		mesh->AddVertexData(1.0f);
		mesh->AddVertexData(color[i]);
		mesh->AddVertexData(1.0f);
	}

	mesh->SetNumElements(6);
	mesh->CreateMesh();
}

void GenerateBackgroundMeshType1(Engine::Mesh* mesh)
{
	mesh->AddAttribute(4);

	std::vector<glm::vec2> pos{
		glm::vec2(-0.5f, -sqrt(3) / 6.0f),
		glm::vec2(0.5f, -sqrt(3) / 6.0f),
		glm::vec2(0.0f,  sqrt(3) / 3.0f)
	};

	for (int i = 0; i < 3; i++)
	{
		mesh->AddVertexData(pos[i]);
		mesh->AddVertexData(0.0f);
		mesh->AddVertexData(1.0f);
	}

	mesh->SetNumElements(3);
	mesh->CreateMesh();
}

void GenerateBackgroundMeshType2(Engine::Mesh* mesh)
{
	mesh->AddAttribute(4);

	std::vector<float> building_widths = std::vector<float>();
	std::vector<float> building_heights = std::vector<float>();
	float x_pos = -4 * SKYLINE_SIZE, x_pos_next;
	float y_pos = -3 * SKYLINE_SIZE;
	
	while (x_pos < 4 * SKYLINE_SIZE)
	{
		float w = randf(SKYLINE_BUILDING_AVG_WIDTH / 3, SKYLINE_BUILDING_AVG_WIDTH * 3);
		float h = randf(SKYLINE_BUILDING_AVG_WIDTH * 4, SKYLINE_BUILDING_AVG_WIDTH * 12);
		x_pos += w;
		building_widths.push_back(w);
		building_heights.push_back(h);
	}

	x_pos = -4 * SKYLINE_SIZE;
	for (int i = 0; i < building_widths.size(); i++)
	{
		x_pos_next = x_pos + building_widths[i];

		mesh->AddVertexData(glm::vec4(
			x_pos, y_pos + building_heights[i], SKYLINE_Z, 1.0f
		));
		mesh->AddVertexData(glm::vec4(
			x_pos, y_pos, SKYLINE_Z, 1.0f
		));
		mesh->AddVertexData(glm::vec4(
			x_pos_next, y_pos, SKYLINE_Z, 1.0f
		));
		mesh->AddVertexData(glm::vec4(
			x_pos, y_pos + building_heights[i], SKYLINE_Z, 1.0f
		));
		mesh->AddVertexData(glm::vec4(
			x_pos_next, y_pos, SKYLINE_Z, 1.0f
		));
		mesh->AddVertexData(glm::vec4(
			x_pos_next, y_pos + building_heights[i], SKYLINE_Z, 1.0f
		));

		x_pos = x_pos_next;
	}

	mesh->SetNumElements(building_widths.size() * 6);
	mesh->CreateMesh();
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    GLFWwindow* window;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1024, 768, "Homework 1 - Snowflake Animation (20170737)", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << std::endl;
        exit(1);
    }

	srand((unsigned int)time(0));

    Fractal fractal = Fractal();
    Animation* animation = new Animation();

    Engine::Camera* mainCamera = new Engine::Camera();

    // Create meshes
    Engine::Mesh* mesh = new Engine::Mesh();
    fractal.GenerateSnowflake(mesh);
	
	Engine::Mesh* backgroundMesh = new Engine::Mesh();
	GenerateGradientRectangle(backgroundMesh);

	Engine::Mesh* triangleParticleMesh = new Engine::Mesh();
	Engine::Mesh* skylineMesh = new Engine::Mesh();
	GenerateBackgroundMeshType1(triangleParticleMesh);
	GenerateBackgroundMeshType2(skylineMesh);

	// Load shader programs into materials
	DefaultMaterial *material = new DefaultMaterial();
	material->CreateMaterial();
	PerVertexColorMaterial *pvMaterial = new PerVertexColorMaterial();
	pvMaterial->CreateMaterial();
    
	// Create renderObjects;
	Engine::RenderObject *backgroundObject = new Engine::RenderObject(backgroundMesh, pvMaterial);
	Engine::RenderObject *skylineObject = new Engine::RenderObject(skylineMesh, material);

	std::vector<FireworksParticle *> fireworks = std::vector<FireworksParticle *>();
	std::vector<FireworksParticle *>::iterator it_fp;

	std::vector<Snowflake *> snowflakes = std::vector<Snowflake *>();
	std::vector<Snowflake *>::iterator it_snowflake;
	
    mainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 15.0f));

	float timestamp = glfwGetTime(), deltaTime;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Render opaque objects */

		backgroundObject->Render(mainCamera);

		material->UpdateColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		skylineObject->Render(mainCamera);

		// Enable transparency and alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

        /* Render (possibly) transparent objects */
		for (auto fp : fireworks)
		{
			material->UpdateColor(fp->GetColor());
			fp->Render(mainCamera);
		}

		for (auto snowflake: snowflakes)
		{
			// update material to each snowflake's color
			material->UpdateColor(snowflake->GetColor());
			
			// render the snowflake
			snowflake->Render(mainCamera);
		}

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		deltaTime = glfwGetTime() - timestamp;
		timestamp = glfwGetTime();

        // Apply constant animation (1.0f is default value. (1.0s) you should change this value for supporting consistent animation)
        animation->Animate(deltaTime);
		for (auto fp : fireworks)
		{
			fp->Animate(deltaTime);
		}

		// Create more snowflakes...
		if (randf(0.0f, 1.0f) < SNOWFLAKE_RATE * deltaTime)
		{
			Snowflake *snowflake = new Snowflake(mesh, material);
			float rotateAngle = randf(-PI, PI), scale = 0.05f + 1.0f / randf(0.8f, 20.0f);

			snowflake->SetPosition(glm::vec3(randf(-10.0f, 10.0f), randf(5.0f, 15.0f), randf(0.0f, 3.0f)));
			snowflake->SetOrientation(glm::rotate(glm::mat4(1.0f), rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f)));
			snowflake->SetScale(glm::vec3(scale, scale, 1.0f));
			snowflake->SetColor(glm::vec4(randf(0.3f, 1.0f), randf(0.3f, 1.0f), randf(0.3f, 1.0f), 0.0f));
			snowflake->RandomizeProperties();

			snowflakes.push_back(snowflake);
			animation->AddSnowflake(snowflake);
		}

		// Create some fireworks...
		if (randf(0.0f, 1.0f) < FIREWORK_RATE * deltaTime)
		{
			glm::vec3 color = glm::vec3(randf(0.0f, 1.0f), randf(0.0f, 1.0f), randf(0.0f, 1.0f));
			glm::vec2 pos = glm::vec2(
				randf(-FIREWORK_MARGIN * 4, FIREWORK_MARGIN * 4), 
				randf(-FIREWORK_MARGIN * 3, FIREWORK_MARGIN * 3) + FIREWORK_Y_MARGIN
			);
			for (int i = 0; i < 48; i++)
			{
				FireworksParticle *p = new FireworksParticle(triangleParticleMesh, material);
				p->RandomizeProperties(color);
				p->SetPosition(glm::vec3(pos, FIREWORK_Z));
				fireworks.push_back(p);
			}
		}

		// Clean up removed objects
		for (it_snowflake = snowflakes.begin(); it_snowflake != snowflakes.end(); )
		{
			if ((*it_snowflake)->GetFlagRemove())
			{
				animation->RemoveSnowflake(*it_snowflake);
				delete *it_snowflake;
				it_snowflake = snowflakes.erase(it_snowflake);
			}
			else
			{
				it_snowflake++;
			}
		}

		for (it_fp = fireworks.begin(); it_fp != fireworks.end(); )
		{
			if ((*it_fp)->GetFlagRemove())
			{
				delete *it_fp;
				it_fp = fireworks.erase(it_fp);
			}
			else
			{
				it_fp++;
			}
		}

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

	/* Clean up */
    delete mainCamera;
	for (auto snowflake : snowflakes)
	{
		delete snowflake;
	}
	snowflakes.clear();
	for (auto fp : fireworks)
	{
		delete fp;
	}
	fireworks.clear();

    glfwTerminate();
    return 0;

}