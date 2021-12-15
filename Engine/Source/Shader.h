#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	//The program ID
/// <summary>
/// The identifier
/// </summary>
	unsigned int ID;
public:
	// constructor reads and builds the shader

/// <summary>
/// Initializes a new instance of the <see cref="Shader" /> class.
/// </summary>
/// <param name="vertexPath">The vertex path.</param>
/// <param name="fragmentPath">The fragment path.</param>
/// <param name="geometryPath">The geometry path.</param>
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	/// <summary>
	/// Uses this instance.
	/// </summary>
	/// use/activate the shader
	void use();
	/// <summary>
	/// Sets the bool.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="value">if set to <c>true</c> [value].</param>
	/// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	/// <summary>
	/// Sets the int.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="value">The value.</param>
	void setInt(const std::string& name, int value) const;
	/// <summary>
	/// Sets the float.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="value">The value.</param>
	void setFloat(const std::string& name, float value) const;
	/// <summary>
	/// Sets the mat4.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="value">The value.</param>
	void setMat4(const std::string& name, glm::mat4 value) const;
	/// <summary>
	/// Sets the vec3.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="value">The value.</param>
	void setVec3(const std::string& name, glm::vec3 value) const;

	/// <summary>
	/// Gets the shader identifier.
	/// </summary>
	/// <returns></returns>
	unsigned int getShaderID() const;
};


inline Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// 1. retrieve the vertex / fragment source code from filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment, geometry;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// print errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// vertex Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}
	}


	ID = glCreateProgram();

	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	glValidateProgram(ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}


inline void Shader::use()
{
	glUseProgram(ID);
}


inline void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}


inline void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


inline void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


inline void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


inline void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}



inline unsigned int Shader::getShaderID() const
{
	return ID;
}