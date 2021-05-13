#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
		GLfloat ambientIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir );

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionIntensityLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

