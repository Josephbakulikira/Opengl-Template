#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"

class Shader
{
public:
	Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDirectionLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetSpecularIntensity();
	GLuint GetEyePositionLocation();

	void SetDirectionalLight(DirectionalLight * dLight);
	void SetPointLight(PointLight* pLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();

	~Shader();
private:
	int pointLightCount;

	GLuint shaderID, uniformProjection, 
		uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess;
	GLuint uniformPointLightCount;
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

