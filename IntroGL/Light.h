#pragma once

#include<GL\glew.h>
#include<glm\glm.hpp>

class Light
{
public:
	Light();

	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat dIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, 
				GLfloat diffuseIntensityLocation, GLfloat directionIntensityLocation);

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	
	GLfloat diffuseIntensity;
};

