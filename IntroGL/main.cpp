#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"
#include "PointLight.h"


// Window dimensions
const GLint WIDTH = 1366, HEIGHT = 768;
const float toRadians = 3.14159265 / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
GLuint VBO, VAO, IBO;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;
float angle = 0.0f;
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader code
static const char* vShader = "Shaders/vertexShader.glsl";

// Fragment Shader
static const char* fShader = "Shaders/fragmentShader.glsl";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount,
	GLfloat* vertices, unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < indiceCount; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1], vertices[in1+ 2] -vertices[in0+2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}
	for (size_t i = 0; i < verticesCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateShaders() {
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CreateObjects()
{

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//x     y     z       u     v       nx   ny    nz
		-1.0f, -1.0f, -0.6f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,	 0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.6f,   1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	 0.5f, 1.0f,  0.0f, 0.0f, 0.0f
	};
	
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);
	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.1f, 0.3f,
								0.0f, 0.0f, -1.0f );
	
	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(0.0f, 1.0f, 1.0f,
								0.1f, 1.0f,
								2.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.1f, 0.4f,
								4.0f, 0.0f, 0.0f,
								0.3f, 0.3f, 0.1f);
	pointLightCount++;
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;
	GLuint uniformEyePosition = 0;

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.1f, 100.0f);
	

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime(); //SDL_GetPermanceCounter
		deltaTime = now - lastTime; // (now - lastTime) * 1000 / SDL_GetPermanceFrequency
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXchange(), mainWindow.getYchange());

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}
		if (angle >= 360) {
			angle -= 360;
		}

		if (sizeDirection) {
			curSize += 0.0001f;
		}
		else {
			curSize -= 0.0001f;
		}

		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();

		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensity();
		uniformShininess = shaderList[0].GetShininessLocation();

		//mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLight(pointLights, pointLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


		glm::mat4 model(1.0f);
		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		

		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.5f, 2.0f, -2.5f));
		model = glm::rotate(model, 20.0f, glm::vec3(0.0f, 1.0f, 0.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
		angle += 0.001f;
	}

	return 0;
}