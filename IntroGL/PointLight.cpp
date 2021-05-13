#include "PointLight.h"

PointLight::PointLight():Light() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}
PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
						GLfloat ambientIntensity, GLfloat dIntensity, 
						GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, 
						GLfloat con, GLfloat lin, GLfloat exp): Light(red, green, blue, ambientIntensity, dIntensity)
{
	position = glm::vec3(x_pos, y_pos, z_pos);
	constant = con;
	linear = lin;
	exponent = exp;
}
void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, 
						GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}
PointLight::~PointLight() {

}