#pragma once
#include "Light.h"

class PointLight :
    public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat dIntensity,
                GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,GLfloat con, GLfloat lin, GLfloat exp);
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
        GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation,
        GLfloat exponentLocation);
    ~PointLight();
private:
    glm::vec3 position;
    // ax^2 + bx + c --> x => distance , b=> linear, a=> exponent, c=> constant
    GLfloat constant, linear, exponent;

};

