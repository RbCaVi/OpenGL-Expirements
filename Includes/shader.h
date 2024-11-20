#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//the programs ID
	unsigned int ID;

	//constructer to build & read the shader
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//retrives the vertex/fragment source code from filepath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		//ensures ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//opens the files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//read file's buffer content into the streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//closes the files
			vShaderFile.close();
			fShaderFile.close();
			//convert stream into text
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//compile shaders

		unsigned int vertex, fragment;
		int success;
		char infolog[512];

		//vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		//vertex compile errors
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infolog);
			std::cout << "ERROR::SHADER::VERTEX:COMPILATION_FAILED\n" << infolog << std::endl;
		}

		//frag shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetProgramiv(fragment, GL_LINK_STATUS, &success);
		//frag compile errors
		if (!success)
		{
			glGetProgramInfoLog(fragment, 512, NULL, infolog);
			std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infolog << std::endl;
		}

		//making the shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infolog);
			std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED" << infolog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	//use/activate the shader
	void use()
	{
		glUseProgram(ID);
	}

	// utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
 
	bool getBool(const std::string& name) 
	{
  bool value;
		glGetnUniformiv(ID, glGetUniformLocation(ID, name.c_str()), 1, (int)value);
  return value;
	}
	int getInt(const std::string& name) 
	{
  int value;
		glGetnUniformiv(ID, glGetUniformLocation(ID, name.c_str()), 1, value);
  return value;
	}
	float getFloat(const std::string& name) 
	{
  float value;
		glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 1, &value);
  return value;
	}
	glm::vec2& getVec2(const std::string& name) 
	{
  glm::vec2& value;
		glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 2, &value[0]);
  return value;
	}
	glm::vec3& getVec3(const std::string& name) 
	{
  glm::vec3& value;
		glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 3, &value[0]);
  return value;
	}
	glm::vec4& getVec4(const std::string& name) 
	{
  glm::vec4& value;
		glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 4, &value[0]);
  return value;
	}
	glm::mat2& getMat2(const std::string& name) 
	{
  glm::mat2& mat;
		glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 4, &mat[0][0]);
  return mat;
	}
	glm::mat3& getMat3(const std::string& name) 
	{
  glm::mat3& mat;
		glGetnUniformfv(ID, glGetUniformLocation(ID, name.c_str()), 9, &mat[0][0]);
  return mat;
	}
	glm::mat4& getMat4(const std::string& name) 
	{
  glm::mat4& mat;
		glGetnUniformfv(glGetUniformLocation(ID, name.c_str()), 1, 16, &mat[0][0]);
  return mat;
	}
};

#endif // !SHADER_H
