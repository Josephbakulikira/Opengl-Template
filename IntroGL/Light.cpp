#include "Light.h"

#include<GL\glew.h>
#include<glm\glm.hpp>

Light::Light() {
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}
Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat dIntensity) {
	colour = glm::vec3(red, green, blue);
	this->ambientIntensity = ambientIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light() {

}